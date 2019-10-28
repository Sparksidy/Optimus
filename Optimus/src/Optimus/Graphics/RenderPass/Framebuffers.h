#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>

namespace OP
{
	class SwapChain;
	class RenderPass;
	class LogicalDevice;

	class OPTIMUS_API Framebuffers
	{
	public:
		Framebuffers(const LogicalDevice* device, const SwapChain* swapchain, const RenderPass* renderpass);

		~Framebuffers();

		const inline std::vector<VkFramebuffer>& GetFramebuffers()const { return m_Framebuffers; }

	private:
		std::vector<VkFramebuffer> m_Framebuffers;
	};
}