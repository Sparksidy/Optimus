#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class SwapChain;
	class LogicalDevice;
	class OPTIMUS_API RenderPass
	{
	public:
		class Subpass
		{
		public:

			Subpass(VkPipelineBindPoint bp, std::vector<VkAttachmentReference> colorAttachmentReferences/*TODO: Depth Attachment*/):
				m_ColorAttachmentReferences(std::move(colorAttachmentReferences))
			{
				m_subPassDescription.pipelineBindPoint = bp;
				m_subPassDescription.colorAttachmentCount = static_cast<uint32_t>(m_ColorAttachmentReferences.size());
				m_subPassDescription.pColorAttachments = m_ColorAttachmentReferences.data();
			}

			const inline VkSubpassDescription& GetSubpassDescription() const { return m_subPassDescription; }
			
		private:
			std::vector<VkAttachmentReference> m_ColorAttachmentReferences;
			VkSubpassDescription m_subPassDescription;
			//TODO: depth attachment references
		};
		
		RenderPass(const SwapChain* swapchain, const LogicalDevice* device);
		~RenderPass();

		inline operator const VkRenderPass& () const{ return m_RenderPass; }

	private:
		VkRenderPass m_RenderPass;
		const SwapChain* m_SwapChain; //TODO: Move to the renderer
		const LogicalDevice* m_Device;
	};
}