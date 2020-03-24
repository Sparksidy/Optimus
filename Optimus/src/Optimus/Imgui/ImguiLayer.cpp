#include <pch.h>
#include <Optimus/Imgui/ImguiLayer.h>

#include <imgui.h>
#include <Platforms/ImguiVulkanRenderer.h>
#include <Platforms/ImguiGLFW.h>

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <Optimus/Application.h>
#include <Optimus/Window.h>
#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Descriptor/DescriptorPool.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>


namespace OP
{
	static void check_vk_result(VkResult err)
	{
		if (err == 0) return;
		OP_CORE_INFO("VkResult {0}", err);
		if (err < 0)
		{
			std::terminate();
		}

	}

	void ImguiLayer::FrameRender()
	{
		size_t img = GET_GRAPHICS_SYSTEM()->GetImageIndex();
		if (GET_GRAPHICS_SYSTEM()->SwapchainRebuild())
		{
			GET_GRAPHICS_SYSTEM()->SetRecreateSwapchain(false);

			for (auto framebuffer : m_ImguiFrameBuffers)
			{
				vkDestroyFramebuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), framebuffer, nullptr);
			}

			_createFramebuffers();
			img = 0;
		}

		
		{
			OP_VULKAN_ASSERT(vkResetCommandPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiCommandPool, 0);
			VkCommandBufferBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			OP_VULKAN_ASSERT(vkBeginCommandBuffer, m_ImguiCommandBuffer[img], &info);
		}
		{
			VkRenderPassBeginInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			info.renderPass = m_ImguiRenderPass;
			info.framebuffer = m_ImguiFrameBuffers[img];
			info.renderArea.extent.width = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width;
			info.renderArea.extent.height = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height;
			info.clearValueCount = 1;
			VkClearValue col;
			ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
			memcpy(&col.color.float32[0], &clear_color, 4 * sizeof(float));
			info.pClearValues = &col;
			vkCmdBeginRenderPass(m_ImguiCommandBuffer[img], &info, VK_SUBPASS_CONTENTS_INLINE);
		}

		// Record Imgui Draw Data and draw funcs into command buffer
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), m_ImguiCommandBuffer[img]);

		
		vkCmdEndRenderPass(m_ImguiCommandBuffer[img]);
		OP_VULKAN_ASSERT(vkEndCommandBuffer, m_ImguiCommandBuffer[img]);
	}


#define OP_BIND_FN(x) std::bind(&ImguiLayer::x, this, std::placeholders::_1)

	ImguiLayer::ImguiLayer() :Layer("ImGui Layer")
	{
	}

	ImguiLayer::~ImguiLayer()
	{
	}

	void ImguiLayer::OnAttach()
	{
		//Setting up a new descriptor pool
		_createIMGUIDescriptorPool();

		//Creating a separate Render pass for Imgui
		_createIMGUIRenderPass();

		//Set up command pool and command buffers
		_createIMGUICommandPoolsAndBuffers();

		//Set up framebuffers for IMGUI
		_createFramebuffers();

		//Init imgui Context
		_initIMGUIContext();

		//Vulkan Initialization for Imgui
		_initIMGUIVulkan();

		//Load the Fonts
		_uploadFonts();

		OP_CORE_INFO("Imgui layer initialized successfully");
	}

	void ImguiLayer::_createIMGUIRenderPass()
	{
		VkAttachmentDescription attachment = {};
		attachment.format = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapchainImageFormat();
		attachment.samples = VK_SAMPLE_COUNT_1_BIT;
		attachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
		attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE; 
		attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference color_attachment = {};
		color_attachment.attachment = 0;
		color_attachment.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &color_attachment;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;  // or VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		info.attachmentCount = 1;
		info.pAttachments = &attachment;
		info.subpassCount = 1;
		info.pSubpasses = &subpass;
		info.dependencyCount = 1;
		info.pDependencies = &dependency;

		OP_VULKAN_ASSERT(vkCreateRenderPass, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &info, nullptr, &m_ImguiRenderPass);
		OP_CORE_INFO("Imgui: Render Pass has been created successfully");
	}

	void ImguiLayer::_createIMGUIDescriptorPool()
	{
		VkDescriptorPoolSize pool_sizes[] =
		{
			{ VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
			{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
			{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
			{ VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
		};

		VkDescriptorPoolCreateInfo pool_info = {};
		pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		pool_info.maxSets = 1000 * IM_ARRAYSIZE(pool_sizes);
		pool_info.poolSizeCount = (uint32_t)IM_ARRAYSIZE(pool_sizes);
		pool_info.pPoolSizes = pool_sizes;
		OP_VULKAN_ASSERT(vkCreateDescriptorPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &pool_info, nullptr, &m_ImguiDescriptorPool);

		OP_CORE_INFO("Imgui: Descriptor Pool Created");
	}

	void ImguiLayer::_initIMGUIContext()
	{
		IMGUI_CHECKVERSION();

		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;


		ImGui::StyleColorsDark();
	}

	void ImguiLayer::_initIMGUIVulkan()
	{
		ImGui_ImplGlfw_InitForVulkan(Application::Get().GetWindow().GetWindowPointer(), true);

		ImGui_ImplVulkan_InitInfo init_info = {};
		init_info.Instance = GET_GRAPHICS_SYSTEM()->GetInstance();
		init_info.PhysicalDevice = GET_GRAPHICS_SYSTEM()->GetPhysicalDevice();
		init_info.Device = GET_GRAPHICS_SYSTEM()->GetLogicalDevice();
		init_info.QueueFamily = GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsFamily();
		init_info.Queue = GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue();
		init_info.PipelineCache = VK_NULL_HANDLE;
		init_info.DescriptorPool = m_ImguiDescriptorPool;
		init_info.Allocator = nullptr;
		init_info.CheckVkResultFn = check_vk_result;
		ImGui_ImplVulkan_Init(&init_info, m_ImguiRenderPass);
	}

	void ImguiLayer::_uploadFonts()
	{
		{
			OP_VULKAN_ASSERT(vkResetCommandPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiCommandPool, 0);
			VkCommandBufferBeginInfo begin_info = {};
			begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			OP_VULKAN_ASSERT(vkBeginCommandBuffer, m_ImguiCommandBuffer[GET_GRAPHICS_SYSTEM()->GetImageIndex()], &begin_info);

			ImGui_ImplVulkan_CreateFontsTexture(m_ImguiCommandBuffer[GET_GRAPHICS_SYSTEM()->GetImageIndex()]);

			VkSubmitInfo end_info = {};
			end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			end_info.commandBufferCount = 1;
			end_info.pCommandBuffers = &m_ImguiCommandBuffer[GET_GRAPHICS_SYSTEM()->GetImageIndex()];
			OP_VULKAN_ASSERT(vkEndCommandBuffer, m_ImguiCommandBuffer[GET_GRAPHICS_SYSTEM()->GetImageIndex()]);
			OP_VULKAN_ASSERT(vkQueueSubmit, GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue(), 1, &end_info, VK_NULL_HANDLE);

			OP_VULKAN_ASSERT(vkDeviceWaitIdle, GET_GRAPHICS_SYSTEM()->GetLogicalDevice());
			ImGui_ImplVulkan_InvalidateFontUploadObjects();

			OP_CORE_INFO("Imgui: Fonts loaded successfully");
		}
	}

	void createCommandPool(VkCommandPool* commandPool, VkCommandPoolCreateFlags flags)
	{
		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.queueFamilyIndex = GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsFamily();
		commandPoolCreateInfo.flags = flags;

		OP_VULKAN_ASSERT(vkCreateCommandPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, commandPool);
	}

	void createCommandBuffers(VkCommandBuffer* commandBuffer, uint32_t commandBufferCount, VkCommandPool& commandPool)
	{
		VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};
		commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		commandBufferAllocateInfo.commandPool = commandPool;
		commandBufferAllocateInfo.commandBufferCount = commandBufferCount;
		OP_VULKAN_ASSERT(vkAllocateCommandBuffers, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &commandBufferAllocateInfo, commandBuffer);
	}

	void ImguiLayer::_createIMGUICommandPoolsAndBuffers()
	{
		createCommandPool(&m_ImguiCommandPool, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		m_ImguiCommandBuffer.resize(GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImageViews().size());

		createCommandBuffers(m_ImguiCommandBuffer.data(), static_cast<uint32_t>(m_ImguiCommandBuffer.size()), m_ImguiCommandPool);

		OP_CORE_INFO("Imgui: Command Buffers and Command Pools have been created");
	}

	void ImguiLayer::_createFramebuffers()
	{
		m_ImguiFrameBuffers.resize(GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImageViews().size());
		{
			VkImageView attachment[1];
			VkFramebufferCreateInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			info.renderPass = m_ImguiRenderPass;
			info.attachmentCount = 1;
			info.pAttachments = attachment;
			info.width = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width;
			info.height = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height;
			info.layers = 1;

			for (uint32_t i = 0; i < GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImageViews().size(); i++)
			{
				attachment[0] = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImageViews()[i];
				OP_VULKAN_ASSERT(vkCreateFramebuffer, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &info, nullptr, &m_ImguiFrameBuffers[i]);
			}
		}

	}

	void ImguiLayer::OnDetach()
	{
		OP_CORE_INFO("Imgui: Clearing up Imgui resources! ");
		for (auto framebuffer : m_ImguiFrameBuffers)
		{
			vkDestroyFramebuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), framebuffer, nullptr);
		}

		vkDestroyRenderPass(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiRenderPass, nullptr);

		vkFreeCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiCommandPool, static_cast<uint32_t>(m_ImguiCommandBuffer.size()), m_ImguiCommandBuffer.data());
		vkDestroyCommandPool(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiCommandPool, nullptr);

		vkDestroyDescriptorPool(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_ImguiDescriptorPool, nullptr);

		// Resources to destroy when the program ends
		ImGui_ImplVulkan_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

	}

	void ImguiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2(app.GetWindow().GetWindowWidth(), app.GetWindow().GetWindowHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? time - m_time : (1.0f / 60.0f);
		m_time = time;

		bool show_demo_window = true;
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::ShowDemoWindow(&show_demo_window);
		ImGui::Render();

		FrameRender();
	}

	void ImguiLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<MouseButtonPressed>(OP_BIND_FN(OnMouseButtonPressedEvent));

		dispatcher.Dispatch<MouseButtonReleased>(OP_BIND_FN(OnMouseButtonReleasedEvent));

		dispatcher.Dispatch<MouseScroll>(OP_BIND_FN(OnMouseScrollEvent));

		dispatcher.Dispatch<MouseMove>(OP_BIND_FN(OnMouseMoveEvent));

		dispatcher.Dispatch<KeyPressedEvent>(OP_BIND_FN(OnKeyPressedEvent));

		dispatcher.Dispatch<KeyReleasedEvent>(OP_BIND_FN(OnKeyReleasedEvent));

		dispatcher.Dispatch<KeyTypedEvent>(OP_BIND_FN(OnKeyTypedEvent));
	}

	bool ImguiLayer::OnMouseButtonPressedEvent(MouseButtonPressed& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return true;
	}

	bool ImguiLayer::OnMouseButtonReleasedEvent(MouseButtonReleased& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return true;
	}

	bool ImguiLayer::OnMouseScrollEvent(MouseScroll& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH = e.GetXOffset();
		io.MouseWheel = e.GetYOffset();

		return true;
	}

	bool ImguiLayer::OnMouseMoveEvent(MouseMove& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return true;
	}

	bool ImguiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		return true;
	}

	bool ImguiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return true;
	}

	bool ImguiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = e.GetKeyCode();
		if (keyCode > 0 && keyCode < 0x10000)
		{
			io.AddInputCharacter((unsigned short)keyCode);
		}

		return true;
	}

	void ImguiLayer::ShowSimpleOverlay(bool* p_open)
	{
		const float DISTANCE = 10.0f;
		static int corner = 0;
		ImGuiIO& io = ImGui::GetIO();
		if (corner != -1)
		{
			ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
			ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		}
		ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background
		if (ImGui::Begin("Example: Simple overlay", p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
		{
			ImGui::Text("Simple overlay\n" "in the corner of the screen.\n" "(right-click to change position)");
			ImGui::Separator();
			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse Position: <invalid>");
			if (ImGui::BeginPopupContextWindow())
			{
				if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
				if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
				if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
				if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
				if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
				if (p_open && ImGui::MenuItem("Close"))* p_open = false;
				ImGui::EndPopup();
			}
		}
		ImGui::End();
	}

}


