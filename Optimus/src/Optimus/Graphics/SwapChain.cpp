#include <pch.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Window.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Log.h>
#include "SwapChain.h"

namespace OP
{
	SwapChain::SwapChain(const Surface* surface, const LogicalDevice* logicalDevice):m_Surface(surface), m_LDevice(logicalDevice)
	{
		_initSwapChain();
	}

	SwapChain::~SwapChain()
	{
		vkDestroySwapchainKHR(m_LDevice->GetLogicalDevice(), m_Swapchain, nullptr);
	}

	VkExtent2D SwapChain::_chooseSwapExtent()
	{
		if (m_Surface->GetCapabilities().currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return m_Surface->GetCapabilities().currentExtent;
		}
		else
		{
			Window& window = Application::Get().GetWindow();

			VkExtent2D actualExtent = { window.GetWindowWidth(), window.GetWindowHeight() };

			actualExtent.width = std::max(m_Surface->GetCapabilities().minImageExtent.width, std::min(m_Surface->GetCapabilities().maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(m_Surface->GetCapabilities().minImageExtent.height, std::min(m_Surface->GetCapabilities().maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	void SwapChain::_initSwapChain()
	{
		VkExtent2D extent = _chooseSwapExtent();

		uint32_t imageCount = m_Surface->GetCapabilities().minImageCount + 1;
		if (m_Surface->GetCapabilities().maxImageCount > 0 &&
			imageCount > m_Surface->GetCapabilities().maxImageCount)
		{
			imageCount = m_Surface->GetCapabilities().maxImageCount;
		}

		VkSwapchainCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		createInfo.surface = m_Surface->GetSurface();
		createInfo.minImageCount = imageCount;
		createInfo.imageFormat = m_Surface->GetFormat().format;
		createInfo.imageColorSpace = m_Surface->GetFormat().colorSpace;
		createInfo.imageExtent = extent;
		createInfo.imageArrayLayers = 1;
		createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

		auto graphicsFamily{ m_LDevice->GetGraphicsFamily() };
		auto presentFamily{ m_LDevice->GetPresentFamily() };
		if (graphicsFamily != presentFamily)
		{
			std::array<uint32_t, 2> queueFamily{ graphicsFamily, presentFamily };
			createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			createInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamily.size());
			createInfo.pQueueFamilyIndices = queueFamily.data();
		}

		createInfo.preTransform = m_Surface->GetCapabilities().currentTransform;
		createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		createInfo.presentMode = m_Surface->GetPresentMode();
		createInfo.clipped = VK_TRUE;
		createInfo.oldSwapchain = VK_NULL_HANDLE;

		OP_VULKAN_ASSERT(vkCreateSwapchainKHR, m_LDevice->GetLogicalDevice(), &createInfo, nullptr, &m_Swapchain);
		vkGetSwapchainImagesKHR(m_LDevice->GetLogicalDevice(), m_Swapchain, &imageCount, m_SwapChainImages.data());
		m_SwapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(m_LDevice->GetLogicalDevice(), m_Swapchain, &imageCount, m_SwapChainImages.data());


		m_SwapChainImageFormat = m_Surface->GetFormat().format;
		m_SwapChainExtent = extent;
		
		OP_CORE_INFO("Swapchain Created Successfully");

		_createImageViews();

	}

	void SwapChain::_createImageViews()
	{
		m_SwapChainImageViews.resize(m_SwapChainImages.size());

		for (size_t i = 0; i < m_SwapChainImages.size(); i++) {
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

			OP_VULKAN_ASSERT(vkCreateImageView, m_LDevice->GetLogicalDevice(), &createInfo, nullptr, &m_SwapChainImageViews[i]);
		}

		OP_CORE_INFO("Image Views created");

	}
}