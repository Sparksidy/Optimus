#pragma once
#include <vulkan/vulkan.h>
#include <optional>

#include <Optimus/Core.h>

namespace OP
{
	class Instance;
	class PhysicalDevice;
	class Surface;

	class OPTIMUS_API LogicalDevice
	{
	public:
		 LogicalDevice(const Instance* instance, const PhysicalDevice* physicalDevice, const Surface* surface);

		~LogicalDevice();

	private:
		void _createLogicalDevice();
		void _createQueueIndices();

	private:
		const Instance *m_Instance;
		const PhysicalDevice *m_PhysicalDevice;
		const Surface *m_Surface;
		VkDevice m_LogicalDevice;

		VkPhysicalDeviceFeatures m_enabledFeatures;

		VkQueueFlags m_supportedQueues;
		uint32_t m_graphicsFamily;
		uint32_t m_presentFamily;
		uint32_t m_computeFamily;
		uint32_t m_transferFamily;

		VkQueue m_graphicsQueue;
		VkQueue m_presentQueue;
		VkQueue m_computeQueue;
		VkQueue m_transferQueue;
	};
}