#include <pch.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Descriptors/DescriptorSet.h>
#include <Optimus/Graphics/Pipelines/Pipeline.h>

namespace OP
{
	DescriptorSet::DescriptorSet(const Pipeline& pipeline)
		:m_Pipelinelayout(pipeline.GetPipelineLayout()),
		m_PipelineBindPoint(pipeline.GetPipelineBindPoint()),
		m_DescriptorPool(pipeline.GetDescriptorPool())
	{
		VkDescriptorSetLayout layouts[1] = { pipeline.GetDescriptorSetLayout() };

		VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
		descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorSetAllocateInfo.descriptorPool = m_DescriptorPool;
		descriptorSetAllocateInfo.descriptorSetCount = 1;
		descriptorSetAllocateInfo.pSetLayouts = layouts;
		OP_VULKAN_ASSERT(vkAllocateDescriptorSets, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &descriptorSetAllocateInfo, &m_DescriptorSet);

	}
	DescriptorSet::~DescriptorSet()
	{
		vkFreeDescriptorSets(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_DescriptorPool, 1, &m_DescriptorSet);

		OP_CORE_INFO("Freeing Descriptor Pool and Sets");
	}
	void DescriptorSet::Update(std::vector<VkWriteDescriptorSet>& descriptorWrites)
	{
		vkUpdateDescriptorSets(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
	void DescriptorSet::BindDescriptor(const CommandBuffer& commandBuffer)
	{
		vkCmdBindDescriptorSets(commandBuffer, m_PipelineBindPoint, m_Pipelinelayout, 0, 1, &m_DescriptorSet, 0, nullptr);
	}
}