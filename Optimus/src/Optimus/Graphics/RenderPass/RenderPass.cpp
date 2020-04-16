#include <pch.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/RenderStage.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Log.h>

namespace OP
{
	RenderPass::RenderPass(const RenderStage& renderStage, VkFormat depthFormat, VkFormat surfaceFormat, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT)
	{
		/*VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = m_SwapChain->GetSwapchainImageFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		//TODO: Use subpass class
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo = {}; 
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		
		OP_VULKAN_ASSERT(vkCreateRenderPass, *device, &renderPassInfo, nullptr, &m_RenderPass);
		OP_CORE_INFO("Renderpass created for a single color attachment");*/

		//Create Renderpass Attachment Descriptions
		std::vector<VkAttachmentDescription> attachmentDescriptions;

		for (const auto& attachment : renderStage.GetAttachments())
		{
			auto attachmentSamples = attachment.IsMultisampled() ? samples : VK_SAMPLE_COUNT_1_BIT;

			VkAttachmentDescription attachmentDescription = {};
			attachmentDescription.samples = attachmentSamples;
			attachmentDescription.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR; 
			attachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			attachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			switch (attachment.GetType())
			{
			case Attachment::Type::Image:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
				attachmentDescription.format = attachment.GetFormat();
				break;
			case Attachment::Type::Depth:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				attachmentDescription.format = depthFormat;
				break;
			case Attachment::Type::Swapchain:
				attachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
				attachmentDescription.format = surfaceFormat;
				break;
			}

			attachmentDescriptions.emplace_back(attachment);
		}

		//Create subpasses and their dependencies
		std::vector<std::unique_ptr<SubpassDescription>> subpasses;
		std::vector<VkSubpassDependency> dependencies;

		for (const auto& subpassType : renderStage.GetSubpasses())
		{
			std::vector<VkAttachmentReference> subpassColorAttachments;
			std::optional<uint32_t> depthAttachment;

			for (const auto& attachmentBinding : subpassType.GetAttachmentBindings())
			{
				auto attachment = renderStage.GetAttachment(attachmentBinding);
				if (!attachment)
				{
					OP_CORE_ERROR("Failed to find a renderpass attachment bound to: {0}", attachmentBinding);
					continue;
				}

				if (attachment->GetType() == Attachment::Type::Depth)
				{
					depthAttachment = attachment->GetBinding();
				}

				VkAttachmentReference attachmentReference = {};
				attachmentReference.attachment = attachment->GetBinding();
				attachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

				subpassColorAttachments.emplace_back(attachmentReference);
			}

			subpasses.emplace_back(std::make_unique<SubpassDescription>(VK_PIPELINE_BIND_POINT_GRAPHICS, subpassColorAttachments, depthAttachment));

			VkSubpassDependency subpassDependency = {};
			subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
			subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			if (subpassType.GetBinding() == renderStage.GetSubpasses().size())
			{
				subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
			}
			else
			{
				subpassDependency.dstSubpass = subpassType.GetBinding();
			}

			if (subpassType.GetBinding() == 0)
			{
				subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
				subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
				subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}
			else
			{
				subpassDependency.srcSubpass = subpassType.GetBinding() - 1;
			}

			dependencies.emplace_back(subpassDependency);

		}

		std::vector<VkSubpassDescription> subpassDescriptions;
		subpassDescriptions.reserve(subpasses.size());
		for (const auto& subpass : subpasses)
		{
			subpassDescriptions.emplace_back(subpass->GetSubpassDescription());
		}

		// Creates the render pass.
		VkRenderPassCreateInfo renderPassCreateInfo = {};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = static_cast<uint32_t>(attachmentDescriptions.size());
		renderPassCreateInfo.pAttachments = attachmentDescriptions.data();
		renderPassCreateInfo.subpassCount = static_cast<uint32_t>(subpassDescriptions.size());
		renderPassCreateInfo.pSubpasses = subpassDescriptions.data();
		renderPassCreateInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassCreateInfo.pDependencies = dependencies.data();
		OP_VULKAN_ASSERT(vkCreateRenderPass, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &renderPassCreateInfo, nullptr, &m_RenderPass);
		OP_CORE_INFO("Renderpass has been created");
	}

	RenderPass::~RenderPass()
	{
		OP_CORE_INFO("Destroying Renderpass in destructor...");
		vkDestroyRenderPass(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_RenderPass, nullptr);
	}

}

