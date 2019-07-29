#include <pch.h>
#include <Optimus/Utilities/Macros.h>
#include <Optimus/Window.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Log.h>

namespace OP
{
	Surface::Surface(const Instance * instance, const PhysicalDevice * physicalDevice) :
		m_Instance(instance),
		m_PhysicalDevice(physicalDevice),
		m_Surface(VK_NULL_HANDLE),
		m_Capabilities({}),
		m_Format(),
		m_PresentMode()
	{
		if (Application::Get().GetWindow().CreateSurface(*m_Instance, nullptr, &m_Surface) != VK_SUCCESS)
		{
			OP_FATAL("Unable to create GLFW Surface");
			std::terminate();
		}

		_initSurface();

		OP_CORE_INFO("Surface is created\n");
	}

	Surface::~Surface()
	{
		vkDestroySurfaceKHR(*m_Instance, m_Surface, nullptr);
	}

	void Surface::_initSurface()
	{
		//Surface Capabilities
		OP_VULKAN_ASSERT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR, *m_PhysicalDevice, m_Surface, &m_Capabilities);

		//Surface Formats
		_chooseSurfaceFormat();

		//Presentation modes
		_choosePresentationMode();
	}

	void Surface::_chooseSurfaceFormat()
	{
		//Surface Formats
		uint32_t surfaceFormatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(*m_PhysicalDevice, m_Surface, &surfaceFormatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(*m_PhysicalDevice, m_Surface, &surfaceFormatCount, surfaceFormats.data());

		if ((surfaceFormatCount == 1) && (surfaceFormats[0].format == VK_FORMAT_UNDEFINED))
		{
			m_Format.format = VK_FORMAT_B8G8R8A8_UNORM;
			m_Format.colorSpace = surfaceFormats[0].colorSpace;
		}
		else
		{
			// Iterate over the list of available surface format and
			// check for the presence of VK_FORMAT_B8G8R8A8_UNORM
			bool found_B8G8R8A8_UNORM = false;

			for (auto &&surfaceFormat : surfaceFormats)
			{
				if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_UNORM)
				{
					m_Format.format = surfaceFormat.format;
					m_Format.colorSpace = surfaceFormat.colorSpace;
					found_B8G8R8A8_UNORM = true;
					break;
				}
			}

			// In case VK_FORMAT_B8G8R8A8_UNORM is not available
			// select the first available color format
			if (!found_B8G8R8A8_UNORM)
			{
				m_Format.format = surfaceFormats[0].format;
				m_Format.colorSpace = surfaceFormats[0].colorSpace;
			}
		}

		//Store in available formats to access later by swapchain
		_availableFormats = surfaceFormats;
	}

	void Surface::_choosePresentationMode()
	{
		//Presentation Modes
		uint32_t presentModeCount;
		vkGetPhysicalDeviceSurfacePresentModesKHR(*m_PhysicalDevice, m_Surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes{ presentModeCount };
		if (presentModeCount != 0)
		{
			vkGetPhysicalDeviceSurfacePresentModesKHR(*m_PhysicalDevice, m_Surface, &presentModeCount, presentModes.data());
		}

		VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;
		for (const auto& availablePresentMode : presentModes) {
			if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
				m_PresentMode = availablePresentMode;
				break;
			}
			else if (availablePresentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) {
				bestMode = availablePresentMode;
				break;
			}
		}
		m_PresentMode = bestMode;

		_availablePresentModes = presentModes;
	}

}

