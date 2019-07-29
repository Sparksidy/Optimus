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

		const VkPresentModeKHR &GetPresentMode() const { return  m_PresentMode; }

		const std::vector<VkSurfaceFormatKHR> GetAvailableFormats() const { return _availableFormats; }

		const std::vector<VkPresentModeKHR> GetAvailablePresentModes() const { return _availablePresentModes;}

	private:
		void _initSurface();

		void _chooseSurfaceFormat();

		void _choosePresentationMode();


	private:
		const Instance *m_Instance;
		const PhysicalDevice *m_PhysicalDevice;

		VkSurfaceKHR m_Surface{VK_NULL_HANDLE};

		//Surface Properties : Selected
		VkSurfaceCapabilitiesKHR m_Capabilities{ VK_NULL_HANDLE };
		VkSurfaceFormatKHR m_Format{};
		VkPresentModeKHR m_PresentMode{};

		//Surface Properties : Available
		std::vector<VkSurfaceFormatKHR> _availableFormats;
		std::vector<VkPresentModeKHR> _availablePresentModes;
	};
}