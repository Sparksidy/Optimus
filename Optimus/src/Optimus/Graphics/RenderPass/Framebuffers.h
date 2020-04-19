#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/RenderPass/SwapChain.h>
namespace OP
{
	class RenderStage;
	class OPTIMUS_API Framebuffers
	{
	public:
		Framebuffers(const RenderStage& renderStage, const SwapChain& swapchain);

		~Framebuffers();

		const inline std::vector<VkFramebuffer>& GetFramebuffers()const { return m_Framebuffers; }
		const VkFramebuffer GetActiveFrameBuffer(uint32_t index)const { return m_Framebuffers[index]; }

	private:
		//TODO: Image2D Attachments
		std::vector<VkFramebuffer> m_Framebuffers;
	};
}