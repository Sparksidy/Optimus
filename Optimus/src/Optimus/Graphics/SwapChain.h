#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


namespace OP
{
	class Surface;
	class LogicalDevice;

	class OPTIMUS_API SwapChain {
	public:
		SwapChain(const Surface* surface, const LogicalDevice* logicalDevice);
		~SwapChain();

	private:
		inline bool _isSwapChainAdequate() { return !m_Surface->GetAvailableFormats().empty() && !m_Surface->GetAvailablePresentModes().empty(); }

		VkExtent2D _chooseSwapExtent();

		void _initSwapChain();

	private:

		VkSwapchainKHR m_Swapchain{ VK_NULL_HANDLE };
		std::vector<VkImage> m_SwapChainImages;
		VkFormat  m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		//TODO: Graphics/RenderSystem should hold these
		const Surface* m_Surface;
		//Logical Device
		const LogicalDevice* m_LDevice;

	};

}
