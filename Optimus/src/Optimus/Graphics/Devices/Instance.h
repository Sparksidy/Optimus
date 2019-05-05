#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "Optimus/Core.h"

namespace OP
{
	class OPTIMUS_API Instance
	{
	public:
		Instance();

		~Instance();

		/*GETTERS*/

		inline operator const VkInstance &() const { return m_Instance; }

		inline const std::vector<const char*>& GetInstanceExtensions()const { return m_InstanceExtensions; }

		inline const std::vector<const char*>& GetInstanceValidationLayers()const { return m_ValidationLayers; }

		inline const std::vector<const char*>& GetDeviceExtensions()const { return m_DeviceExtensions; }

	private:
		/**
		 * Creates a Vulkan Instance
		 */
		void _createInstance();


		/**
		 * Returns the Instance Extensions
		 */
		const std::vector<const char*>& _getRequiredExtensions();


		/**
		 * Returns all the Instance Extension Properties
		 */
		const std::vector<VkExtensionProperties>& _getInstanceExtensionProperties()const;


		/**
		 * Checks if the Validation Layer is present with the instance
		 */
		bool _checkValidationLayerSupport()const;


		/**
		 * Sets up the Debug layer with the instance
		 */
		void _setupDebugLayer();

	private:
		VkInstance m_Instance;

		std::vector<const char*> m_InstanceExtensions;

		static const std::vector<const char*> m_ValidationLayers;
		static const std::vector<const char*> m_DeviceExtensions;

		VkDebugUtilsMessengerEXT m_DebugMessenger;

#ifdef NDEBUG
		const bool m_EnableValidationLayers = false;
#else
		const bool m_EnableValidationLayers = true;
#endif
	};
}//namespace OP
