#include <pch.h>
#include <Optimus/Graphics/Descriptor/DescriptorSet.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Graphics/Descriptor/DescriptorSetLayout.h>
#include <Optimus/Graphics/Descriptor/DescriptorPool.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Buffers/Buffer.h>

namespace OP
{
	DescriptorSet::DescriptorSet()
	{
		_createDescriptorSets();
	}
	DescriptorSet::~DescriptorSet()
	{
	}
	void DescriptorSet::_createDescriptorSets()
	{
		size_t swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		std::vector<VkDescriptorSetLayout> layouts(swapChainImages, GET_GRAPHICS_SYSTEM()->GetDescriptorSetLayout().GetDescriptorSetLayout());
	
		VkDescriptorSetAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = GET_GRAPHICS_SYSTEM()->GetDescriptorPool().GetDescriptorPool();
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImages);
		allocInfo.pSetLayouts = layouts.data();

		m_DescriptorSets.resize(swapChainImages);

		OP_VULKAN_ASSERT(vkAllocateDescriptorSets,GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, m_DescriptorSets.data());

		for (size_t i = 0; i < swapChainImages; i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = GET_GRAPHICS_SYSTEM()->GetBuffer().GetUniformBuffer()[i];
			bufferInfo.offset = 0;
			bufferInfo.range = GET_GRAPHICS_SYSTEM()->GetBuffer().GetSizeOfUBO();

			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;

			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;

			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pImageInfo = nullptr; // Optional
			descriptorWrite.pTexelBufferView = nullptr; // Optional

			vkUpdateDescriptorSets(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), 1, &descriptorWrite, 0, nullptr);
		}
	}
}