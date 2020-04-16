#include <pch.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Graphics/Buffers/Buffer.h>
#include <Optimus/Graphics/Descriptor/DescriptorSet.h>
#include<Optimus/Application.h>



namespace OP
{
	CommandBuffer::CommandBuffer(bool begin = true, VkQueueFlagBits queueType = VK_QUEUE_GRAPHICS_BIT, VkCommandBufferLevel bufferLevel = VK_COMMAND_BUFFER_LEVEL_PRIMARY):
		m_QueueType(queueType)
	{
		m_CommandPool = GET_GRAPHICS_SYSTEM()->GetCommandPool();

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = *m_CommandPool;
		allocInfo.level = bufferLevel;
		allocInfo.commandBufferCount = 1;

		OP_VULKAN_ASSERT(vkAllocateCommandBuffers, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, &m_CommandBuffer);

		if (begin)
			Begin();
	}
	CommandBuffer::~CommandBuffer()
	{
		vkFreeCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), *m_CommandPool, 1, &m_CommandBuffer);
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
	}
	void CommandBuffer::Submit(const VkSemaphore& waitSemaphore, const VkSemaphore& signalSemaphore, VkFence fence)
	{
		if (m_Running)
			End();

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &m_CommandBuffer;

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

		VkQueue queue = GetQueue();
		OP_VULKAN_ASSERT(vkQueueSubmit, queue, 1, &submitInfo, fence);
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
	//void CommandBuffer::createCommandBuffers()
	//{
		//Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));
		//m_CommandBuffers.resize(graphics->GetFramebuffers().GetFramebuffers().size());

		//VkCommandBufferAllocateInfo allocInfo = {};
		//allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		//allocInfo.commandPool = graphics->GetCommandPool();
		//allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		//allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		//if (vkAllocateCommandBuffers(graphics->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		//{
		//	throw std::runtime_error("failed to allocate command buffers!");
		//}

		//for (size_t i = 0; i < m_CommandBuffers.size(); i++)
		//{
		//	VkCommandBufferBeginInfo beginInfo = {};
		//	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		//	if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
		//		throw std::runtime_error("failed to begin recording command buffer!");
		//	}

		//	VkRenderPassBeginInfo renderPassInfo = {};
		//	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		//	renderPassInfo.renderPass = graphics->GetRenderPass();
		//	renderPassInfo.framebuffer = graphics->GetFramebuffers().GetFramebuffers()[i];
		//	renderPassInfo.renderArea.offset = { 0, 0 };
		//	renderPassInfo.renderArea.extent = graphics->GetSwapchain().GetSwapChainExtent();

		//	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		//	renderPassInfo.clearValueCount = 1;
		//	renderPassInfo.pClearValues = &clearColor;

		//	vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		//			vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,graphics->GetGraphicsPipeline());

		//			VkBuffer vertexBuffers[] = { GET_GRAPHICS_SYSTEM()->GetBuffer() };
		//			VkDeviceSize offsets[] = { 0 };

		//			vkCmdBindVertexBuffers(m_CommandBuffers[i], 0, 1, vertexBuffers, offsets);
		//			vkCmdBindIndexBuffer(m_CommandBuffers[i], GET_GRAPHICS_SYSTEM()->GetBuffer().GetIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

		//			//Binding the right descriptor set for each swap chain image
		//			vkCmdBindDescriptorSets(m_CommandBuffers[i], 
		//				VK_PIPELINE_BIND_POINT_GRAPHICS, 
		//				GET_GRAPHICS_SYSTEM()->GetGraphicsPipeline().GetPipelineLayout(),
		//				0, 
		//				1, 
		//				&GET_GRAPHICS_SYSTEM()->GetDescriptorSet().GetDescriptorSet()[i],
		//				0, 
		//				nullptr);

		//			vkCmdDrawIndexed(m_CommandBuffers[i], static_cast<uint32_t>(GET_GRAPHICS_SYSTEM()->GetBuffer().GetIndices().size()), 1, 0, 0, 0);

		//	vkCmdEndRenderPass(m_CommandBuffers[i]);

		//	if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
		//	{
		//		throw std::runtime_error("failed to record command buffer!");
		//	}
		//}

		//OP_CORE_INFO("Command Buffers created");

	//}
}