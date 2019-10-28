#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/Devices/Surface.h>

namespace OP
{
	class Surface;
	class LogicalDevice;

	class OPTIMUS_API SwapChain
	{
	public:
		SwapChain(const Surface* surface, const LogicalDevice* logicalDevice);
		~SwapChain();

		const VkFormat& GetSwapchainImageFormat() const { return m_SwapChainImageFormat; }

		const VkExtent2D GetSwapChainExtent()const { return m_SwapChainExtent; }

		const std::vector<VkImageView> GetSwapChainImageViews()const { return m_SwapChainImageViews; }

	private:
		inline bool _isSwapChainAdequate() { return !m_Surface->GetAvailableFormats().empty() && !m_Surface->GetAvailablePresentModes().empty(); }

		VkExtent2D _chooseSwapExtent();

		void _initSwapChain();

		void _createImageViews();

	private:

		VkSwapchainKHR m_Swapchain{ VK_NULL_HANDLE };
		VkFormat  m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;

		std::vector<VkImageView> m_SwapChainImageViews;
		std::vector<VkImage> m_SwapChainImages;

		//TODO: Graphics/RenderSystem should hold these
		const Surface* m_Surface;
		//Logical Device
		const LogicalDevice* m_LDevice;

	};

}
