#include "pch.h"

#include "PhysicalDevice.h"
#include "Instance.h"
#include "Optimus/Log.h"

namespace OP
{
	PhysicalDevice::PhysicalDevice(const Instance & instance):m_Instance(&instance)
	{
		_pickPhysicalDevice();
	}

	PhysicalDevice::~PhysicalDevice()
	{

	}

	const VkPhysicalDeviceFeatures & PhysicalDevice::GetFeatures() const
	{
		return m_PhysicalDeviceFeatures;
	}

	void PhysicalDevice::_pickPhysicalDevice()
	{
		_choosePhysicalDevice();

		if (m_PhysicalDevice == VK_NULL_HANDLE)
		{
			OP_FATAL("Unable to find a suitable GPU");
			std::terminate();
		}

		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &m_PhysicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &m_PhysicalDeviceFeatures);

		OP_CORE_TRACE("Selected Physical Device: {0}, {1}\n", m_PhysicalDeviceProperties.deviceName, m_PhysicalDeviceProperties.deviceID);
	}

	void PhysicalDevice::_choosePhysicalDevice()
	{
		uint32_t physicalDevicesCount = 0;
		vkEnumeratePhysicalDevices(*m_Instance, &physicalDevicesCount, nullptr);

		if (physicalDevicesCount == 0)
		{
			OP_FATAL("Failed to find the GPU's with Vulkan support");
		}

		std::vector<VkPhysicalDevice> physicalDevices(physicalDevicesCount);
		vkEnumeratePhysicalDevices(*m_Instance, &physicalDevicesCount, physicalDevices.data());

		std::multimap<int, VkPhysicalDevice> candidates;

		for (const auto& device : physicalDevices)
		{
			int score = _rateDeviceSuitability(device);
			candidates.insert(std::make_pair(score, device));
		}

		// Check if the best candidate is suitable at all
		if (candidates.rbegin()->first > 0)
		{
			m_PhysicalDevice = candidates.rbegin()->second;
		}
		else
		{
			throw std::runtime_error("failed to find a suitable GPU!");
		}

	}

	int PhysicalDevice::_rateDeviceSuitability(const VkPhysicalDevice& device)
	{
		int32_t score = 0;

		// Checks if the requested extensions are supported.
		uint32_t extensionPropertyCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionPropertyCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionPropertyCount, extensionProperties.data());

		// Iterates through all extensions requested.
		for (const char *currentExtension : m_Instance->GetDeviceExtensions())
		{
			bool extensionFound = false;

			// Checks if the extension is in the available extensions.
			for (const auto &extension : extensionProperties)
			{
				if (strcmp(currentExtension, extension.extensionName) == 0)
				{
					extensionFound = true;
					break;
				}
			}

			// Returns a score of 0 if this device is missing a required extension.
			if (!extensionFound)
			{
				return 0;
			}
		}
	}

}


