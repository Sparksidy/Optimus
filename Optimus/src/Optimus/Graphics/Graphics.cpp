#include "pch.h"
#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/SwapChain.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/GraphicsPipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
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
	}

	Graphics::~Graphics()
	{
		cleanupSwapChain();

		//Destroy these in its own classes
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_RenderFinishedSemaphore[i], nullptr);
			vkDestroySemaphore(m_LogicalDevice->GetLogicalDevice(), m_ImageAvailableSemaphore[i], nullptr);
			vkDestroyFence(m_LogicalDevice->GetLogicalDevice(), m_InFlightFences[i], nullptr);
		}
	}

	bool Graphics::Initialize()
	{
		m_SwapChain = std::make_unique<SwapChain>(m_Surface.get(), m_LogicalDevice.get());

		m_Renderpass = std::make_unique<RenderPass>(m_SwapChain.get(), m_LogicalDevice.get());

		m_GraphicsPipeline = std::make_unique<GraphicsPipeline>();

		m_Framebuffers = std::make_unique<Framebuffers>(m_LogicalDevice.get(), m_SwapChain.get(), m_Renderpass.get());

		m_CommandPool = std::make_unique<CommandPool>(m_LogicalDevice.get());

		m_CommandBuffers = std::make_unique<CommandBuffer>();

		if(!recreatingSwapchain)
			createSyncObjects();

		m_isInitialized = true;

		return m_isInitialized;
	}

	void Graphics::Update()
	{
		drawFrame();
		
		//Drawing operations are asynchronous, wait for logical device to finish its operation before cleanup
		vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());
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
			if (vkCreateSemaphore(m_LogicalDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_ImageAvailableSemaphore[i]) != VK_SUCCESS ||
				vkCreateSemaphore(m_LogicalDevice->GetLogicalDevice(), &semaphoreInfo, nullptr, &m_RenderFinishedSemaphore[i]) != VK_SUCCESS ||
				vkCreateFence(m_LogicalDevice->GetLogicalDevice(), &fenceInfo, nullptr, &m_InFlightFences[i]) != VK_SUCCESS)
			{

				throw std::runtime_error("failed to create semaphores!");
			}
		}

		OP_CORE_INFO("Sync objects created");
	}
	
	void Graphics::cleanupSwapChain()
	{
		OP_CORE_INFO("Destroying Framebuffers...");
		m_Framebuffers.reset();

		OP_CORE_INFO("Freeing Command buffers...");
		m_CommandBuffers.reset();

		OP_CORE_INFO("Destroying Pipeline objects...");
		m_GraphicsPipeline.reset();

		OP_CORE_INFO("Destroying RenderPasses...");
		m_Renderpass.reset();

		OP_CORE_INFO("Destroying Image Views and swapchain...");
		m_SwapChain.reset();
	}

	void Graphics::recreateSwapchain()
	{
		if (recreatingSwapchain)
		{
			OP_CORE_INFO("Recreating swapchain");

			vkDeviceWaitIdle(m_LogicalDevice->GetLogicalDevice());

			cleanupSwapChain();

			Initialize();

			recreatingSwapchain = false;
		}
	}

	void Graphics::drawFrame()
	{
		vkWaitForFences(m_LogicalDevice->GetLogicalDevice(), 1, &m_InFlightFences[m_CurrentFrame], VK_TRUE, UINT64_MAX);
		
		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(m_LogicalDevice->GetLogicalDevice(), m_SwapChain->GetSwapchain(), UINT64_MAX, m_ImageAvailableSemaphore[m_CurrentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			//recreate swapchain
			recreatingSwapchain = true; //For sync objects
			recreateSwapchain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		//Check if a previous frame is using this image
		if (m_ImagesInFlight[imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(m_LogicalDevice->GetLogicalDevice(), 1, &m_ImagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}

		m_ImagesInFlight[imageIndex] = m_InFlightFences[m_CurrentFrame];

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { m_ImageAvailableSemaphore[m_CurrentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffers->GetCommandBuffer()[imageIndex]; 

		VkSemaphore signalSemaphores[] = { m_RenderFinishedSemaphore[m_CurrentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(m_LogicalDevice->GetLogicalDevice(), 1, &m_InFlightFences[m_CurrentFrame]);

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
	}
}