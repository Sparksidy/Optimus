#include <pch.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Log.h>


namespace OP
{
	CommandPool::CommandPool(const std::thread::id& id) :m_ThreadID(id)
	{
		auto graphicsFamily = GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsFamily();

		VkCommandPoolCreateInfo commandPoolCreateInfo = {};
		commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		commandPoolCreateInfo.queueFamilyIndex = graphicsFamily;

		OP_VULKAN_ASSERT(vkCreateCommandPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &commandPoolCreateInfo, nullptr, &m_CommandPool);

		OP_CORE_INFO("Command Pool created");
	}

	CommandPool::~CommandPool()
	{
		OP_CORE_INFO("Destroying command pool");
		vkDestroyCommandPool(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_CommandPool, nullptr);
	}

}

