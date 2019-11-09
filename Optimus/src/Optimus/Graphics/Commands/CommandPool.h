#pragma once
#include <vulkan/vulkan.h>
#include <Optimus/Core.h>

namespace OP
{
	class LogicalDevice;
	class OPTIMUS_API CommandPool
	{
	public:
		
		CommandPool(const LogicalDevice* device);

		~CommandPool();

		operator const VkCommandPool& ()const { return m_CommandPool; }

		inline const VkCommandPool& GetCommandPool()const { return m_CommandPool; }
	private:
		VkCommandPool m_CommandPool;
	};
}

