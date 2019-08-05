#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Application.h>

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

		static GraphicsSystem* Get() { return &Application::Get().GetGraphicsSystem(); }

		const SwapChain* GetSwapChain()const { return m_Swapchain.get(); }

		const PhysicalDevice& GetPhysicalDevice()const { return *m_PhysicalDevice; }

		const LogicalDevice& GetLogicalDevice()const { return *m_LogicalDevice; }

		const Surface& GetSurface()const { return *m_Surface; }

	private:

		std::unique_ptr<Instance> m_Instance;
		std::unique_ptr<PhysicalDevice> m_PhysicalDevice;
		std::unique_ptr<Surface> m_Surface;
		std::unique_ptr<LogicalDevice> m_LogicalDevice;
		std::unique_ptr<SwapChain> m_Swapchain;
	};
}