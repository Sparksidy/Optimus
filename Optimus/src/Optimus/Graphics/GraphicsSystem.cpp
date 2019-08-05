#include "pch.h"
#include <Optimus/Graphics/GraphicsSystem.h>

#include <Optimus/Graphics/Devices/Instance.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Devices/Surface.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/SwapChain.h>

namespace OP
{
	GraphicsSystem::GraphicsSystem():m_Instance(std::make_unique<Instance>()),
		m_PhysicalDevice(std::make_unique<PhysicalDevice>(*m_Instance.get())),
		m_Surface(std::make_unique<Surface>(m_Instance.get(), m_PhysicalDevice.get())),
		m_LogicalDevice(std::make_unique<LogicalDevice>(m_Instance.get(), m_PhysicalDevice.get(), m_Surface.get())),
		m_Swapchain(std::make_unique<SwapChain>(m_Surface.get(), m_LogicalDevice.get()))
	{
	}

	GraphicsSystem::~GraphicsSystem()
	{

	}

}

