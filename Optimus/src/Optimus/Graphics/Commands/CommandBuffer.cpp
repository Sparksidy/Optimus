#include <pch.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include<Optimus/Application.h>



namespace OP
{
	CommandBuffer::CommandBuffer(bool begin, VkQueueFlagBits queueType, VkCommandBufferLevel bufferLevel):
		m_QueueType(queueType)
	{
		m_CommandPool = GET_GRAPHICS_SYSTEM()->GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;

		OP_VULKAN_ASSERT(vkAllocateCommandBuffers, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, &m_CommandBuffer);


		OP_CORE_TRACE("Command Buffer Created");

		if (begin)
			Begin();
	}
	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), *m_CommandPool, 1, &m_CommandBuffer);

		OP_CORE_TRACE("Destroying Command Buffer");
	}
	void CommandBuffer::Begin(VkCommandBufferUsageFlags usage)
	{
		if (m_Running)
			return;

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = usage;
		OP_VULKAN_ASSERT(vkBeginCommandBuffer, m_CommandBuffer, &beginInfo);
		m_Running = true;
	}
	void CommandBuffer::End()
	{
		if (!m_Running)
			return;

		OP_VULKAN_ASSERT(vkEndCommandBuffer, m_CommandBuffer);
		m_Running = false;
	}
	void CommandBuffer::Submit(const VkSemaphore& waitSemaphore, const VkSemaphore& signalSemaphore, VkFence fence)
	{
		if (m_Running)
			End();

		//Add imgui layer to CB TODO: Use a ifdef of rendering imgui only when debugging
		std::array<VkCommandBuffer, 2> submitCommandBuffers =
		{ 
			m_CommandBuffer, 
			Application::Get().GetImGUILayer().GetImGUICommandBuffer()[GET_GRAPHICS_SYSTEM()->GetSwapchain().GetActiveImageIndex()] 
		};

		
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = static_cast<uint32_t>(submitCommandBuffers.size());
		submitInfo.pCommandBuffers = submitCommandBuffers.data();

		if(waitSemaphore != VK_NULL_HANDLE)
		{ 
			VkPipelineStageFlags waitStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			
			submitInfo.pWaitDstStageMask = &waitStages;
			submitInfo.waitSemaphoreCount = 1;
			submitInfo.pWaitSemaphores = &waitSemaphore;
		}
		
		if (signalSemaphore != VK_NULL_HANDLE)
		{
			submitInfo.signalSemaphoreCount = 1;
			submitInfo.pSignalSemaphores = &signalSemaphore;
		}

		if(fence != VK_NULL_HANDLE)
			OP_VULKAN_ASSERT(vkResetFences, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), 1, &fence);

		OP_VULKAN_ASSERT(vkQueueSubmit, GetQueue(), 1, &submitInfo, fence);
	}

	void CommandBuffer::SubmitIdle()
	{
		if (m_Running)
			End();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

		VkFenceCreateInfo fenceCreateInfo = {};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

		VkFence fence;
		OP_VULKAN_ASSERT(vkCreateFence, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &fenceCreateInfo, nullptr, &fence);

		OP_VULKAN_ASSERT(vkResetFences, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), 1, &fence);

		OP_VULKAN_ASSERT(vkQueueSubmit, GetQueue(), 1, &submitInfo, fence);

		OP_VULKAN_ASSERT(vkWaitForFences, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), 1, &fence, VK_TRUE, std::numeric_limits<uint64_t>::max());

		vkDestroyFence(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), fence, nullptr);
	}
	VkQueue CommandBuffer::GetQueue() const
	{
		switch (m_QueueType)
		{
		case VK_QUEUE_GRAPHICS_BIT:
			return GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue();
		default:
			return nullptr;
			//TODO: Add Compute queue here
		}
	}
}