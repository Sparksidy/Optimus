#pragma once
#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class Instance;
	class PhysicalDevice;

	class OPTIMUS_API Surface
	{
	public:
		Surface(const Instance* instance, const PhysicalDevice* physicalDevice);
		~Surface();

		inline operator const VkSurfaceKHR &() const { return m_Surface; }

		const VkSurfaceKHR &GetSurface() const { return m_Surface; }

		const VkSurfaceCapabilitiesKHR &GetCapabilities() const { return m_Capabilities; }

		const VkSurfaceFormatKHR &GetFormat() const { return m_Format; }

	private:
		void _initSurface();

	private:
		const Instance *m_Instance;
		const PhysicalDevice *m_PhysicalDevice;

		VkSurfaceKHR m_Surface{VK_NULL_HANDLE};
		VkSurfaceCapabilitiesKHR m_Capabilities{};
		VkSurfaceFormatKHR m_Format;
	};
}