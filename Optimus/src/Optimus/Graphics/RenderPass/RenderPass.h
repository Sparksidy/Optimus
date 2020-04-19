#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <optional>

namespace OP
{
	class RenderStage;
	class OPTIMUS_API RenderPass
	{
	public:
		class SubpassDescription
		{
		public:

			SubpassDescription(VkPipelineBindPoint bp, std::vector<VkAttachmentReference> colorAttachmentReferences, const std::optional<uint32_t> &depthAttachment):
				m_ColorAttachmentReferences(std::move(colorAttachmentReferences))
			{
				m_subPassDescription.pipelineBindPoint = bp;
				m_subPassDescription.colorAttachmentCount = static_cast<uint32_t>(m_ColorAttachmentReferences.size());
				m_subPassDescription.pColorAttachments = m_ColorAttachmentReferences.data();

				if (depthAttachment)
				{
					m_depthStencilAttachment.attachment = *depthAttachment;
					m_depthStencilAttachment.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
					m_subPassDescription.pDepthStencilAttachment = &m_depthStencilAttachment;
				}
			}

			const inline VkSubpassDescription& GetSubpassDescription() const { return m_subPassDescription; }
			
		private:
			std::vector<VkAttachmentReference> m_ColorAttachmentReferences;
			VkSubpassDescription m_subPassDescription;
			VkAttachmentReference m_depthStencilAttachment = {};
		};
		
		RenderPass(const RenderStage& renderStage, VkFormat depthFormat, VkFormat surfaceFormat, VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT);
		~RenderPass();

		operator const VkRenderPass& () const { return m_RenderPass; }
		const VkRenderPass& GetRenderpass() const { return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;
	};
}