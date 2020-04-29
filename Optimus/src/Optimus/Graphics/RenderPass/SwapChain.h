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
		explicit SwapChain(const VkExtent2D& displayExtent, const SwapChain* oldSwapChain = nullptr);

		~SwapChain();

		operator const VkSwapchainKHR& () const { return m_Swapchain; }

		VkResult AcquireNextImage(const VkSemaphore& presentCompleteSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

		VkResult QueuePresent(const VkQueue& presentQueue, const VkSemaphore& waitSemaphore = VK_NULL_HANDLE);


		//Getters
		const VkFormat& GetSwapchainImageFormat() const { return m_SwapChainImageFormat; }
		const VkExtent2D GetSwapChainExtent()const { return m_SwapChainExtent; }
		const std::vector<VkImageView> GetSwapChainImageViews()const { return m_SwapChainImageViews; }
		const uint32_t GetImageCount()const { return m_ImageCount; }
		const size_t GetSwapChainImages()const { return m_SwapChainImages.size(); }
		uint32_t GetActiveImageIndex()const { return m_ActiveImageIndex; }
		bool IsSameExtent(const VkExtent2D& other) { return m_SwapChainExtent.width == other.width && m_SwapChainExtent.height == other.height; }
		


	private:
		
		VkExtent2D ChooseSwapChainExtent();

		void _createImageViews(); //TODO: Create an Image representation for this resource

	private:
		VkExtent2D					m_SwapChainExtent;
		VkPresentModeKHR			m_PresentMode;
		uint32_t					m_ImageCount;
		VkSwapchainKHR				m_Swapchain				= VK_NULL_HANDLE;
		VkFormat					m_SwapChainImageFormat;


		std::vector<VkImageView>	m_SwapChainImageViews;
		std::vector<VkImage>		m_SwapChainImages;
		uint32_t					m_ActiveImageIndex;

		VkFence						m_ImageFence = VK_NULL_HANDLE;

	};

}
