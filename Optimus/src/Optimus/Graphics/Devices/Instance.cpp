#include "pch.h"
#include "Instance.h"
#include <Optimus/Log.h>
#include "DebugCallbacks.h"

namespace OP
{
	const std::vector<const char*> Instance::m_ValidationLayers = { "VK_LAYER_LUNARG_standard_validation" };
	const std::vector<const char*> Instance::m_DeviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	//const std::vector<const char *> Instance::m_InstanceExtensions = { VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME };

	Instance::Instance()
	{
		_createInstance();
		_setupDebugLayer();

		OP_CORE_INFO("Vulkan Instance has been created!");
		OP_CORE_INFO("Vulkan Debug Layer is {0}\n", m_EnableValidationLayers);
	}

	Instance::~Instance()
	{
		if (m_EnableValidationLayers)
		{
			OP::Debug::DestroyDebugUtilsMessengerEXT(m_Instance, m_DebugMessenger, nullptr);
		}

		vkDestroyInstance(m_Instance, nullptr);
	}

	void Instance::_createInstance()
	{
		if (m_EnableValidationLayers && !_checkValidationLayerSupport())
		{
			OP_FATAL("Validation requested but not available");
		}

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Renderer";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Optimus Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		if (m_EnableValidationLayers)
		{
			createInfo.enabledLayerCount = static_cast<uint32_t>(m_ValidationLayers.size());
			createInfo.ppEnabledLayerNames = m_ValidationLayers.data();
		}
		else
		{
			createInfo.enabledLayerCount = 0;
		}

		auto extensions = _getRequiredExtensions();
		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();

		OP_VULKAN_ASSERT(vkCreateInstance, &createInfo, nullptr, &m_Instance);

		_getInstanceExtensionProperties();
	}

	const std::vector<const char*>& Instance::_getRequiredExtensions()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;
		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		for (uint32_t i = 0; i < glfwExtensionCount; i++)
		{
			m_InstanceExtensions.emplace_back(glfwExtensions[i]);
		}

		if (m_EnableValidationLayers)
		{
			m_InstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		}

		return m_InstanceExtensions;
	}

	const std::vector<VkExtensionProperties>& Instance::_getInstanceExtensionProperties() const
	{
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensionProperties(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionProperties.data());

		/*OP_CORE_INFO("Vulkan available extensions...");
		for (const auto& extension : extensionProperties)
		{
			OP_CORE_INFO("\t {0}", extension.extensionName);
		}
		*/

		return extensionProperties;
	}

	bool Instance::_checkValidationLayerSupport() const
	{
		uint32_t layerCount;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

		std::vector<VkLayerProperties> availableLayers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

		for (const char* layerName : m_ValidationLayers)
		{
			bool layerFound = false;

			for (const auto& layerProperties : availableLayers) {
				if (strcmp(layerName, layerProperties.layerName) == 0) {
					layerFound = true;
					break;
				}
			}

			if (!layerFound) {
				return false;
			}

		}
		return true;
	}

	void Instance::_setupDebugLayer()
	{
		if (!m_EnableValidationLayers)
			return;

		VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		createInfo.pfnUserCallback = OP::Debug::debugCallback;
		createInfo.pUserData = nullptr;

		OP_VULKAN_ASSERT(OP::Debug::CreateDebugUtilsMessengerEXT, m_Instance, &createInfo, nullptr, &m_DebugMessenger);
	}

}

