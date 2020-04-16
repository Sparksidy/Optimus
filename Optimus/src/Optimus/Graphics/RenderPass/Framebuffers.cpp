#include <pch.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/RenderStage.h>
#include <Optimus/Log.h>

namespace OP
{

	Framebuffers::Framebuffers(const RenderStage& renderStage, const SwapChain& swapchain)
	{
		m_Framebuffers.resize(swapchain.GetSwapChainImageViews().size());

		for (size_t i = 0; i < swapchain.GetSwapChainImageViews().size(); i++)
		{
			std::vector<VkImageView> attachments;
			for (const auto& attachment : renderStage.GetAttachments())
			{
				switch (attachment.GetType())
				{
					case Attachment::Type::Image:
						//TODO: Image2D Attachment Binding
						break;
					case Attachment::Type::Depth:
						//TODO: Depth Attachment
						break;
					case Attachment::Type::Swapchain:
						attachments.emplace_back(swapchain.GetSwapChainImageViews()[i]);
						break;
				}
			}

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = *renderStage.GetRenderPass();
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());;
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = swapchain.GetSwapChainExtent().width;
			framebufferInfo.height = swapchain.GetSwapChainExtent().height;
			framebufferInfo.layers = 1;

			OP_VULKAN_ASSERT(vkCreateFramebuffer, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &framebufferInfo, nullptr, &m_Framebuffers[i]);
		}

		OP_CORE_INFO("Framebuffers created");
	}

	Framebuffers::~Framebuffers()
	{
		if (m_Framebuffers.size() > 0)
		{
			for (auto framebuffer : m_Framebuffers)
			{
				vkDestroyFramebuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), framebuffer, nullptr);
			}
		}
		OP_CORE_INFO("Destroyed Framebuffers in destructor");
	}
}