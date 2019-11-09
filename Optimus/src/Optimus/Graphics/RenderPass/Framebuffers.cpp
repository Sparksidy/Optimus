#include <pch.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h>
#include <Optimus/Graphics/SwapChain.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Log.h>

namespace OP
{

	Framebuffers::Framebuffers(const LogicalDevice* device, const SwapChain* swapchain, const RenderPass* renderpass)
	{
		m_Framebuffers.resize(swapchain->GetSwapChainImageViews().size());

		for (size_t i = 0; i < swapchain->GetSwapChainImageViews().size(); i++)
		{
			VkImageView attachments[] = {
				swapchain->GetSwapChainImageViews()[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = renderpass->GetRenderPass();
			framebufferInfo.attachmentCount = 1;
			framebufferInfo.pAttachments = attachments;
			framebufferInfo.width = swapchain->GetSwapChainExtent().width;
			framebufferInfo.height = swapchain->GetSwapChainExtent().height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(device->GetLogicalDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}

		}

		OP_CORE_INFO("Framebuffers created");
	}

	Framebuffers::~Framebuffers()
	{
		OP_CORE_INFO("Destroying Framebuffers in destructor...");
		if (m_Framebuffers.size() > 0)
		{
			for (auto framebuffer : m_Framebuffers)
			{
				vkDestroyFramebuffer(Application::Get().GetGraphics().GetLogicalDevice()->GetLogicalDevice(), framebuffer, nullptr);
			}
		}
		
	}



}