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

		operator const VkDevice& () const { return m_LogicalDevice; }
		
		const VkDevice& GetLogicalDevice()const { return m_LogicalDevice; }

		const VkPhysicalDeviceFeatures &GetEnabledFeatures() const { return m_enabledFeatures; }

		const VkQueue &GetGraphicsQueue() const { return m_graphicsQueue; }

		const VkQueue &GetPresentQueue() const { return m_presentQueue; }

		const VkQueue &GetComputeQueue() const { return m_computeQueue; }

		const VkQueue &GetTransferQueue() const { return m_transferQueue; }

		const uint32_t &GetGraphicsFamily() const { return m_graphicsFamily; }

		const uint32_t &GetPresentFamily() const { return m_presentFamily; }

		const uint32_t &GetComputeFamily() const { return m_computeFamily; }

		const uint32_t &GetTransferFamily() const { return m_transferFamily; }


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