#include <pch.h>
#include <Optimus/Graphics/Core/Graphics.h>
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
		CreatePipelineCache();
	}

	Graphics::~Graphics()
	{
		//TODO
		OP_VULKAN_ASSERT(vkQueueWaitIdle, m_LogicalDevice->GetGraphicsQueue());

		vkDestroyPipelineCache(*m_LogicalDevice, m_PipelineCache, nullptr);
		OP_CORE_INFO("Destroyed Pipeline Cache");

		for (std::size_t i = 0; i < m_InFlightFences.size(); i++)
		{
			vkDestroyFence(*m_LogicalDevice, m_InFlightFences[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_RenderFinishedSemaphore[i], nullptr);
			vkDestroySemaphore(*m_LogicalDevice, m_ImageAvailableSemaphore[i], nullptr);
		}
		OP_CORE_INFO("Destroyed Fences and Semaphores");

	}

	bool Graphics::Initialize()
	{
		if (!m_Renderer->m_Started)
		{
			ResetRenderStages();
			m_Renderer->Start();
			m_Renderer->m_Started = true;
		}

		return true;
	}

	void Graphics::Update()
	{
		DrawFrame();

		//Wait for the logical device to finish operations before exiting
		vkDeviceWaitIdle(*m_LogicalDevice.get());
	}

	void Graphics::Unload()
	{
	}

	RenderStage* Graphics::GetRenderStage(uint32_t index)
	{
		if (!m_Renderer)
			return nullptr;

		return m_Renderer->GetRenderStage(index);
	}

	const std::shared_ptr<CommandPool>& Graphics::GetCommandPool(const std::thread::id& id)
	{
		auto it = m_CommandPools.find(id);

		if (it != m_CommandPools.end()) {
			return it->second;
		}
		
		return m_CommandPools.emplace(id, std::make_shared<CommandPool>(id)).first->second;
	}

	bool Graphics::StartRenderPass(RenderStage& renderStage)
	{
		if (renderStage.IsOutOfDate())
		{
			//TODO
			RecreatePass(renderStage);
			OP_CORE_INFO("Render Stage is out of date");
			return false;
		}

		auto& commandBuffer = m_CommandBuffers[m_SwapChain->GetActiveImageIndex()];
		if (!commandBuffer->IsRunning())
			commandBuffer->Begin();

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = *renderStage.GetRenderPass();
		renderPassInfo.framebuffer = renderStage.GetActiveFrameBuffer(m_SwapChain->GetActiveImageIndex());
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = m_SwapChain->GetSwapChainExtent();

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(*m_CommandBuffers[m_SwapChain->GetActiveImageIndex()], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		return true;
	}

	void Graphics::EndRenderPass(RenderStage& renderStage)
	{
		auto& commandBuffer = m_CommandBuffers[m_SwapChain->GetActiveImageIndex()];

		vkCmdEndRenderPass(*commandBuffer);

		if (!renderStage.HasSwapChain())
		{
			OP_CORE_INFO("No swapchain for the renderstage");
			return;
		}

		commandBuffer->End();
		commandBuffer->Submit(m_ImageAvailableSemaphore[m_CurrentFrame],
			m_RenderFinishedSemaphore[m_CurrentFrame],
			m_InFlightFences[m_CurrentFrame]);

		auto presentQueue = m_LogicalDevice->GetPresentQueue();
		auto result = m_SwapChain->QueuePresent(presentQueue, m_RenderFinishedSemaphore[m_CurrentFrame]);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			m_FramebufferResized = true; //This will recreate swapchain
		}

		m_CurrentFrame = (m_CurrentFrame + 1) % m_SwapChain->GetImageCount();
	}

	void Graphics::CreatePipelineCache()
	{
		VkPipelineCacheCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		OP_VULKAN_ASSERT(vkCreatePipelineCache, *m_LogicalDevice, &info, nullptr, &m_PipelineCache);

		OP_CORE_INFO("Pipeline Cache Created");
	}

	void Graphics::RecreatePass(RenderStage& renderstage)
	{
		VkExtent2D currExtent = { Application::Get().GetWindow().GetWindowWidth(), Application::Get().GetWindow().GetWindowHeight() };

		OP_VULKAN_ASSERT(vkQueueWaitIdle, m_LogicalDevice->GetGraphicsQueue());

		if (renderstage.HasSwapChain() && (m_FramebufferResized || !m_SwapChain->IsSameExtent(currExtent)))
		{
			RecreateSwapChain();
		}

		renderstage.Rebuild(*m_SwapChain);
		//TODO Attachments Map recreate
	}

	void Graphics::ResetRenderStages()
	{
		//Swapchain , Command Buffers and Command Pool
		RecreateSwapChain();
		
		//Recreate Command Buffers and Command Pool if new swapchain
		if(m_InFlightFences.size() != m_SwapChain->GetImageCount())
			RecreateCommandBuffers();

		//Renderstages(Renderpass & Framebuffers)
		for (const auto& renderStage : m_Renderer->m_RenderStages)
			renderStage->Rebuild(*m_SwapChain);

		//TODO: Attachments map(Descriptors)
	}

	void Graphics::RecreateSwapChain()
	{
		vkDeviceWaitIdle(*m_LogicalDevice);

		VkExtent2D display = { Application::Get().GetWindow().GetWindowWidth(), Application::Get().GetWindow().GetWindowHeight() };

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

	void Graphics::DrawFrame()
	{
		if (!m_Renderer)
		{
			OP_CORE_INFO("Renderer is not set!");
			return;
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

		
		Application::Get().GetImGUILayer().FrameRender();

		Pipeline::Stage Stage;
		for (auto& renderstage : m_Renderer->m_RenderStages)
		{
			renderstage->Update();

			if (!StartRenderPass(*renderstage))
				return;

			auto& commandBuffer = m_CommandBuffers[m_SwapChain->GetActiveImageIndex()];

			for (const auto& subpass : renderstage->GetSubpasses())
			{
				Stage.second = subpass.GetBinding();

				m_Renderer->m_SubRenderHolder.RenderStage(Stage, *commandBuffer);

				if (subpass.GetBinding() != renderstage->GetSubpasses().back().GetBinding())
				{
					vkCmdNextSubpass(*commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
				}
			}
			
			EndRenderPass(*renderstage);
			Stage.first++;
		}
	}

}//namespace OP