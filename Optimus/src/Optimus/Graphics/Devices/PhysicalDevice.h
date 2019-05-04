#pragma once
#include <vulkan/vulkan.hpp>

#include "Optimus/Core.h"

namespace OP
{
	class Instance;

	class OPTIMUS_API PhysicalDevice
	{
	public:
		PhysicalDevice(const Instance& instance);
		~PhysicalDevice();

	private:
		void _pickPhysicalDevice();
		int  _rateDeviceSuitability(const VkPhysicalDevice& device);
		void _choosePhysicalDevice();

	private:
		const Instance* m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
		VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;
	};
}
