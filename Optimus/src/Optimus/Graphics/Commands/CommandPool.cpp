#include "pch.h"
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Log.h>
#include <Optimus/Utilities/Macros.h>


namespace OP
{
	CommandPool::CommandPool(const LogicalDevice* device)
	{
		auto graphicsFamily = device->GetGraphicsFamily();

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;

		if (vkCreateCommandPool(device->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			std::terminate();
		}

		OP_CORE_INFO("Command Pool created");
	}

	CommandPool::~CommandPool()
	{
		OP_CORE_INFO("Destroying command pool");
		auto logicalDevice = Application::Get().GetGraphics().GetLogicalDevice();

		vkDestroyCommandPool(logicalDevice->GetLogicalDevice(), m_CommandPool, nullptr);
	}

}

