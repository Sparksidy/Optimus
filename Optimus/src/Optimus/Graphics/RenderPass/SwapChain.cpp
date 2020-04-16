#include <pch.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Window.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Log.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

namespace OP
{
	SwapChain::SwapChain(const VkExtent2D& displayExtent, const SwapChain* oldSwapChain = nullptr):
		m_SwapChainExtent(displayExtent),
		m_PresentMode(VK_PRESENT_MODE_FIFO_KHR),
		m_ActiveImageIndex(std::numeric_limits<uint32_t>::max())
	{
		//Window Extent
		m_SwapChainExtent = ChooseSwapChainExtent();

		//Image Count
		m_ImageCount = GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().minImageCount + 1;
		if (GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().maxImageCount > 0 &&
			m_ImageCount > GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().maxImageCount)
		{
			m_ImageCount = GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = GET_GRAPHICS_SYSTEM()->GetSurface();
		createInfo.minImageCount = m_ImageCount;
		createInfo.imageFormat = GET_GRAPHICS_SYSTEM()->GetSurface().GetFormat().format;
		createInfo.imageColorSpace = GET_GRAPHICS_SYSTEM()->GetSurface().GetFormat().colorSpace;
		createInfo.imageExtent = m_SwapChainExtent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (oldSwapChain)
			createInfo.oldSwapchain = oldSwapChain->m_Swapchain;

		auto graphicsFamily{ GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsFamily() };
		auto presentFamily{ GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetPresentFamily() };
		if (graphicsFamily != presentFamily)
		{
			std::array<uint32_t, 2> queueFamily{ graphicsFamily, presentFamily };
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
			createInfo.pQueueFamilyIndices = queueFamily.data();
		}

		createInfo.preTransform = GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_PresentMode;
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;


		OP_VULKAN_ASSERT(vkCreateSwapchainKHR, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &createInfo, nullptr, &m_Swapchain);
		vkGetSwapchainImagesKHR(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Swapchain, &m_ImageCount, m_SwapChainImages.data());
		m_SwapChainImages.resize(m_ImageCount);
		vkGetSwapchainImagesKHR(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Swapchain, &m_ImageCount, m_SwapChainImages.data());
		OP_CORE_INFO("Swapchain Created Successfully");

		m_SwapChainImageFormat = GET_GRAPHICS_SYSTEM()->GetSurface().GetFormat().format;
		
		_createImageViews();
	}

	SwapChain::~SwapChain()
	{
		OP_CORE_INFO("Destroying Image views and swapchain in destructor");

		for (auto imageView : m_SwapChainImageViews)
		{
			vkDestroyImageView(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), imageView, nullptr);
		}

		vkDestroySwapchainKHR(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Swapchain, nullptr);
	}

	VkResult SwapChain::AcquireNextImage(const VkSemaphore& presentCompleteSemaphore, VkFence fence)
	{
		if (fence != VK_NULL_HANDLE)
		{
			OP_VULKAN_ASSERT(vkWaitForFences, GET_GRAPHICS_SYSTEM()->GetLogicalDevice, 1, &fence, VK_TRUE, UINT64_MAX);
		}

		return vkAcquireNextImageKHR(GET_GRAPHICS_SYSTEM()->GetLogicalDevice, m_Swapchain, UINT64_MAX, presentCompleteSemaphore, VK_NULL_HANDLE, &m_ActiveImageIndex);
	}

	VkResult SwapChain::QueuePresent(const VkQueue& presentQueue, const VkSemaphore& waitSemaphore)
	{
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &waitSemaphore;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = &m_Swapchain;
		presentInfo.pImageIndices = &m_ActiveImageIndex;

		return vkQueuePresentKHR(GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetPresentQueue(), &presentInfo);
	}

	VkExtent2D SwapChain::ChooseSwapChainExtent()
	{
		Window& window = Application::Get().GetWindow();
		VkExtent2D actualExtent = { static_cast<uint32_t>(window.GetWindowWidth()), static_cast<uint32_t>(window.GetWindowHeight()) };

		if (window.GetWindowWidth() != std::numeric_limits<uint32_t>::max())
		{
			return actualExtent;
		}
		else
		{
			actualExtent.width = std::max(GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().minImageExtent.width, std::min(GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().minImageExtent.height, std::min(GET_GRAPHICS_SYSTEM()->GetSurface().GetCapabilities().maxImageExtent.height, actualExtent.height));
		}
		return actualExtent;
	}

	void SwapChain::_createImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++)
		{
			VkImageViewCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			createInfo.image = m_SwapChainImages[i];
			createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			createInfo.format = m_SwapChainImageFormat;
			createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			createInfo.subresourceRange.baseMipLevel = 0;
			createInfo.subresourceRange.levelCount = 1;
			createInfo.subresourceRange.baseArrayLayer = 0;
			createInfo.subresourceRange.layerCount = 1;

			OP_VULKAN_ASSERT(vkCreateImageView, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]);
		}

		OP_CORE_INFO("Swap Chain Image Views created");

	}
}