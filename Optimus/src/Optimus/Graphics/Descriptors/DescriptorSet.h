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

		const VkDescriptorSet& GetDescriptorSet ()const { return m_DescriptorSet; }

	private:
		VkPipelineLayout m_Pipelinelayout;
		VkPipelineBindPoint m_PipelineBindPoint;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet = VK_NULL_HANDLE;
	};
}