#pragma once
#include <pch.h>
#include <vulkan/vulkan.h>
#include <Optimus/Core.h>

namespace OP
{
	class LogicalDevice;
	class OPTIMUS_API CommandPool
	{
	public:
		
		explicit CommandPool(const std::thread::id &id = std::this_thread::get_id());
		~CommandPool();
		
		inline operator const VkCommandPool& ()const { return m_CommandPool; }

	private:
		VkCommandPool m_CommandPool = VK_NULL_HANDLE;
		std::thread::id m_ThreadID;
	};
}

