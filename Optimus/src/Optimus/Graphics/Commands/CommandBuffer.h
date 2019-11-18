#pragma once
#include<vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	
	class OPTIMUS_API CommandBuffer
	{
	public:

		CommandBuffer();
		~CommandBuffer();

		const std::vector<VkCommandBuffer>& GetCommandBuffer()const { return m_CommandBuffers; }

	private:
		void createCommandBuffers();

	private:
		std::vector<VkCommandBuffer> m_CommandBuffers = {};
	};
}