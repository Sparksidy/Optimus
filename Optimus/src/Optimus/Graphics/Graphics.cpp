#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Buffers/Buffer.h>
#include <Optimus/Graphics/Descriptor/DescriptorPool.h>
#include <Optimus/Graphics/Descriptor/DescriptorSet.h>
#include <Optimus/Graphics/Renderer.h>
#include <Optimus/Graphics/RenderStage.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Application.h>

#include <Optimus/Log.h>


namespace OP
{
	Graphics::Graphics():
		m_Instance(std::make_unique<Instance>()),
		m_PhysicalDevice(std::make_unique<PhysicalDevice>(*m_Instance.get())),
		m_Surface(std::make_unique<Surface>(m_Instance.get(), m_PhysicalDevice.get())),
		m_LogicalDevice(std::make_unique<LogicalDevice>(m_Instance.get(), m_PhysicalDevice.get(), m_Surface.get()))
	{
		//Testing: Since 2D renderer for now so we initialize the quad here;
		m_Quad = std::make_unique<QuadModel>();
	}

	Graphics::~Graphics()
	{
		cleanupSwapChain();

		//Destroy these in its own classes
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
		}

		//Clear the vertex buffer
		m_Buffer.reset();
		OP_INFO("Clearing the vertex buffer");
	}

	bool Graphics::Initialize()
	{
		if(!recreatingSwapchain)
			createSyncObjects();

		m_isInitialized = true;
		return m_isInitialized;
	}

	void Graphics::Update()
	{
		drawFrame();
		
		//Drawing operations are asynchronous, wait for logical device to finish its operation before cleanup
		vkDeviceWaitIdle(*m_LogicalDevice);
	}

	RenderStage* Graphics::GetRenderStage(uint32_t index)
	{
		if (!m_Renderer)
			return nullptr;

		return m_Renderer->GetRenderStage(index);
	}

	void Graphics::resetRenderStages()
	{
		//Swapchain , Command Buffers and Command Pool
		RecreateSwapChain();
		
		//Recreate Command Buffers and Command Pool if new swapchain
		if(m_InFlightFences.size() != m_SwapChain->GetImageCount())
			RecreateCommandBuffers();

		//Renderstages(Renderpass & Framebuffers)
		for (const auto& renderStage : m_Renderer->m_RenderStages)
			renderStage->Rebuild(*m_SwapChain);

		//Attachments map(Descriptors)
	}

	void Graphics::createSyncObjects()
	{
		m_ImageAvailableSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		m_RenderFinishedSemaphore.resize(MAX_FRAMES_IN_FLIGHT);
		m_InFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		m_ImagesInFlight.resize(m_SwapChain->GetSwapChainImageViews().size(), VK_NULL_HANDLE); //TODO: Get Swapchain images

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(*m_LogicalDevice, &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore[i]) != VK_SUCCESS ||
				vkCreateSemaphore(*m_LogicalDevice, &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore[i]) != VK_SUCCESS ||
				vkCreateFence(*m_LogicalDevice, &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
			{

				throw std::runtime_error("failed to create semaphores!");
			}
		}

		OP_CORE_INFO("Sync objects created");
	}
	
	void Graphics::cleanupSwapChain()
	{
		
	}

	void Graphics::recreateSwapchain()
	{
		if (recreatingSwapchain)
		{
			OP_CORE_INFO("Recreating swapchain");

			vkDeviceWaitIdle(*m_LogicalDevice);

			cleanupSwapChain();

			Initialize();

			//recreatingSwapchain = false; //This is getting toggled in IMGUI Layer: TODO: Change this dependency
		}
	}

	void Graphics::RecreateSwapChain()
	{
		vkDeviceWaitIdle(*m_LogicalDevice);

		VkExtent2D display = { Application::Get().GetWindow().GetWindowWidth(), Application::Get().GetWindow().GetWindowHeight() };

		//Recreate swapchain if already exists
		if (m_SwapChain)
		{
			OP_CORE_INFO("Recreating old swapchain");
		}

		m_SwapChain = std::make_unique<SwapChain>(display, m_SwapChain.get());

		RecreateCommandBuffers();
	}

	void Graphics::RecreateCommandBuffers()
	{
		for (std::size_t i = 0; i < m_InFlightFences.size(); i++)
		{
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
		}

		m_ImageAvailableSemaphore.resize(m_SwapChain->GetImageCount());
		m_RenderFinishedSemaphore.resize(m_SwapChain->GetImageCount());
		m_InFlightFences.resize(m_SwapChain->GetImageCount());
		m_CommandBuffers.resize(m_SwapChain->GetImageCount());

		VkSemaphoreCreateInfo semaphoreCreateInfo = {};
		semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (std::size_t i = 0; i < m_InFlightFences.size(); i++)
		{
			OP_VULKAN_ASSERT(vkCreateSemaphore, *m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_ImageAvailableSemaphore[i]);

			OP_VULKAN_ASSERT(vkCreateSemaphore, *m_LogicalDevice, &semaphoreCreateInfo, nullptr, &m_RenderFinishedSemaphore[i]);

			OP_VULKAN_ASSERT(vkCreateFence, *m_LogicalDevice, &fenceCreateInfo, nullptr, &m_InFlightFences[i]);

			m_CommandBuffers[i] = std::make_unique<CommandBuffer>(false);
		}
	}

	void Graphics::drawFrame()
	{
		if (!m_Renderer)
			return;

		if (!m_Renderer->m_Started)
		{
			resetRenderStages();
			m_Renderer->Start();
			m_Renderer->m_Started = true;
		}

		m_Renderer->Update();

		auto result = m_SwapChain->AcquireNextImage(m_ImageAvailableSemaphore[m_CurrentFrame], m_InFlightFences[m_CurrentFrame]);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			OP_CORE_FATAL("Failed to acquire swapchain Image");
		}


		//Adding the IMGUI Command Buffers
		std::array<VkCommandBuffer, 2> submitCommandBuffers = {
			m_CommandBuffers->GetCommandBuffer()[imageIndex],
			Application::Get().GetImGUILayer().GetImGUICommandBuffer()[imageIndex] //TODO: Make this dynamic, loop through all layers and add their command buffers.
		};

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(*m_LogicalDevice, 1, &m_InFlightFences[m_CurrentFrame]);

		if (vkQueueSubmit(m_LogicalDevice->GetGraphicsQueue(), 1, &submitInfo, m_InFlightFences[m_CurrentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { m_SwapChain->GetSwapchain() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;

		presentInfo.pImageIndices = &imageIndex;

		result = vkQueuePresentKHR(m_LogicalDevice->GetPresentQueue(), &presentInfo);

		m_CurrentFrame = (m_CurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
		m_ImageIndex = (m_ImageIndex + 1) % GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImageViews().size();
	}
}