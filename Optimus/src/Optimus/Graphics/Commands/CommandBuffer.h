#pragma once
#include<vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/Commands/CommandPool.h>

namespace OP
{
	class OPTIMUS_API CommandBuffer
	{
	public:

		explicit CommandBuffer(bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT, VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY);
		~CommandBuffer();

		void Begin(VkCommandBufferUsageFlags usage = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);

		void End();

		void Submit(const VkSemaphore& waitSemaphore = VK_NULL_HANDLE, const VkSemaphore& signalSemaphore = VK_NULL_HANDLE, VkFence fence = VK_NULL_HANDLE);

		/*
			Submits the command buffer to the queue and will hold the current 
			thread idle until it has finished
		*/
		void SubmitIdle();

		VkQueue GetQueue()const;
		operator const VkCommandBuffer& ()const { return m_CommandBuffer; }
		const VkCommandBuffer& GetCommandBuffer()const { return m_CommandBuffer; }
		bool IsRunning()const { return m_Running; }

	private:
		std::shared_ptr<CommandPool> m_CommandPool;

		VkCommandBuffer m_CommandBuffer = VK_NULL_HANDLE;
		VkQueueFlagBits m_QueueType;
		bool m_Running = false;
	};
}