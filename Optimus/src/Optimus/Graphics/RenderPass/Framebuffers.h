#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class RenderStage;
	class SwapChain;

	class OPTIMUS_API Framebuffers
	{
	public:
		Framebuffers(const RenderStage& renderStage, const SwapChain& swapchain);

		~Framebuffers();

		const inline std::vector<VkFramebuffer>& GetFramebuffers()const { return m_Framebuffers; }

	private:
		//TODO: Image2D Attachments
		std::vector<VkFramebuffer> m_Framebuffers;
	};
}