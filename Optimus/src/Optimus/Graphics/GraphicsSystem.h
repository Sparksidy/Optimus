#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class Instance;
	class PhysicalDevice;
	class LogicalDevice;
	class Surface;
	class SwapChain;

	class OPTIMUS_API GraphicsSystem
	{
	public:
		GraphicsSystem();
		~GraphicsSystem();

	private:

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<SwapChain> m_Swapchain;
	};
}