#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>

namespace OP
{
	class Instance;

	class OPTIMUS_API PhysicalDevice
	{
	public:
		explicit PhysicalDevice(const Instance& instance);

		~PhysicalDevice();

		const VkPhysicalDeviceFeatures& GetFeatures()const;

		inline operator const VkPhysicalDevice &() const { return m_PhysicalDevice; }

		const VkPhysicalDeviceMemoryProperties& GetMemoryProperties() const { return m_MemProperties; }

		const VkSampleCountFlagBits& GetMsaaSamples() const { return m_MSAASamples; }

	private:
		void _pickPhysicalDevice();
		int  _rateDeviceSuitability(const VkPhysicalDevice& device);
		void _choosePhysicalDevice();

	private:
		const Instance* m_Instance;
		VkPhysicalDevice m_PhysicalDevice;
		VkPhysicalDeviceProperties m_PhysicalDeviceProperties;
		VkPhysicalDeviceFeatures m_PhysicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties m_MemProperties = {};
		VkSampleCountFlagBits m_MSAASamples = VK_SAMPLE_COUNT_1_BIT;
	};
}
