#include <pch.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/RenderPass/Framebuffers.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/RenderPass/RenderPass.h>
#include <Optimus/Graphics/SwapChain.h>
#include <Optimus/Graphics/GraphicsPipeline.h>
#include<Optimus/Application.h>


namespace OP
{
	CommandBuffer::CommandBuffer()
	{
		createCommandBuffers();
	}
	CommandBuffer::~CommandBuffer()
	{
		Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));
		vkFreeCommandBuffers(graphics->GetLogicalDevice(), graphics->GetCommandPool().GetCommandPool(), static_cast<uint32_t>(m_CommandBuffers.size()), m_CommandBuffers.data());
	}
	void CommandBuffer::createCommandBuffers()
	{
		Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));
		m_CommandBuffers.resize(graphics->GetFramebuffers().GetFramebuffers().size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = graphics->GetCommandPool().GetCommandPool();
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)m_CommandBuffers.size();

		if (vkAllocateCommandBuffers(graphics->GetLogicalDevice(), &allocInfo, m_CommandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (size_t i = 0; i < m_CommandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

			if (vkBeginCommandBuffer(m_CommandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = graphics->GetRenderPass().GetRenderPass();
			renderPassInfo.framebuffer = graphics->GetFramebuffers().GetFramebuffers()[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = graphics->GetSwapchain().GetSwapChainExtent();

			VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
			renderPassInfo.clearValueCount = 1;
			renderPassInfo.pClearValues = &clearColor;

			vkCmdBeginRenderPass(m_CommandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdBindPipeline(m_CommandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS,graphics->GetGraphicsPipeline());

			vkCmdDraw(m_CommandBuffers[i], 3, 1, 0, 0);

			vkCmdEndRenderPass(m_CommandBuffers[i]);

			if (vkEndCommandBuffer(m_CommandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to record command buffer!");
			}
		}

		OP_CORE_INFO("Command Buffers created");
		
	}
}