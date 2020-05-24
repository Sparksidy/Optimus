#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class Pipeline;
	class CommandBuffer;
	class OPTIMUS_API DescriptorSet
	{
	public:

		explicit DescriptorSet(const Pipeline& pipeline);

		~DescriptorSet();

		static void Update(std::vector<VkWriteDescriptorSet>& descriptorWrites);

		void BindDescriptor(const CommandBuffer& commandBuffer);

		const VkDescriptorSet& GetDescriptorSet (uint32_t index)const { return m_DescriptorSet[index]; }

	private:
		VkPipelineLayout m_Pipelinelayout;
		VkPipelineBindPoint m_PipelineBindPoint;
		VkDescriptorPool m_DescriptorPool;
		std::vector<VkDescriptorSet> m_DescriptorSet; //descriptor set per swapchain image
	};
}