#pragma once

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include "Optimus/Core.h"

#include <optional>

namespace OP
{
	class OPTIMUS_API Renderer
	{
	public:
		Renderer();
		~Renderer();

		void Init();

	private:
		VkInstance m_Instance;

		#pragma region Validation Layers
		const std::vector<const char*> m_ValidationLayers =
		{
			"VK_LAYER_LUNARG_standard_validation"
		};

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		#pragma endregion

		#pragma region Physical Device

		VkPhysicalDevice m_PhysicalDevice;

#pragma region Queue Families

		struct QueueFamilyIndices
		{
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentationFamily;

			bool isComplete()
			{
				return graphicsFamily.has_value() && presentationFamily.has_value();
			}
		};

#pragma endregion Queue Families

#pragma endregion

		#pragma region Logical Device
		VkDevice m_Logicaldevice;
#pragma endregion Logical Device

		VkQueue m_GraphicsQueue; //Device queues are implicitly are cleaned up when the device is destroyed
		VkQueue m_PresentationQueue;

		VkSurfaceKHR m_Surface;

#pragma region Swapchain
		const std::vector<const char*> m_deviceExtensions
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		struct _SwapChainSupportDetails
		{
			VkSurfaceCapabilitiesKHR capabilities;
			std::vector<VkSurfaceFormatKHR> formats;
			std::vector<VkPresentModeKHR> presentModes;
		};

		VkSwapchainKHR m_SwapChain;
		std::vector<VkImage> m_SwapChainImages;
		VkFormat m_SwapChainImageFormat;
		VkExtent2D m_SwapChainExtent;
		std::vector<VkImageView> m_SwapChainImageViews;
#pragma endregion


	private:
		void Shutdown();
		void _createInstance();
		std::vector<const char*> _getRequiredExtensions();

		void _createSurface();

		#pragma region Validation Layers
		bool _checkValidationLayerSupport();
		static VKAPI_ATTR VkBool32 VKAPI_CALL _debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		void _setupDebugMessenger();
#pragma endregion

		#pragma region Physical Devices
		void _pickPhysicalDevices();
		bool _isDeviceSuitable(VkPhysicalDevice device);

#pragma region Queue Families
		QueueFamilyIndices _findQueueFamilyIndices(VkPhysicalDevice device);

#pragma endregion Queue Families

#pragma endregion Physical Devices

		#pragma region Logical Device
		void _createLogicalDevice();
#pragma endregion Logical Device

		bool _checkForExtensionsSupport(VkPhysicalDevice device);

		_SwapChainSupportDetails _querySwapChainSupport(VkPhysicalDevice device);
		VkSurfaceFormatKHR _chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR _chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D _chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void _createSwapChain();

		void _createImageViews();
	};
}
