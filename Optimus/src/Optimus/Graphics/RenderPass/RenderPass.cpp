#include <pch.h>
#include "RenderPass.h"
#include <Optimus/Graphics/SwapChain.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Log.h>

namespace OP
{
	RenderPass::RenderPass(const SwapChain* swapchain, const LogicalDevice* device):m_SwapChain(swapchain), m_Device(device)
	{
		
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_SwapChain->GetSwapchainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		//TODO: Use subpass class
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkRenderPassCreateInfo renderPassInfo = {}; 
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		
		OP_VULKAN_ASSERT(vkCreateRenderPass, device->GetLogicalDevice(), &renderPassInfo, nullptr, &m_RenderPass);
		OP_CORE_INFO("Renderpass created for a single color attachment");
	}

	RenderPass::~RenderPass()
	{
		OP_CORE_INFO("Destroying Renderpass in destructor...");
		Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));
		vkDestroyRenderPass(graphics->GetLogicalDevice().GetLogicalDevice(), m_RenderPass, nullptr);
	}

}

