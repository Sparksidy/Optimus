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
		OP_CORE_INFO("Descriptor Sets is created");


		for (size_t i = 0; i < swapChainImages; i++)
		{
			VkDescriptorBufferInfo bufferInfo = {};
			bufferInfo.buffer = GET_GRAPHICS_SYSTEM()->GetBuffer().GetUniformBuffer()[i];
			bufferInfo.offset = 0;
			bufferInfo.range = GET_GRAPHICS_SYSTEM()->GetBuffer().GetSizeOfUBO();

			VkDescriptorImageInfo imageInfo = {};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = GET_GRAPHICS_SYSTEM()->GetBuffer().GetTextureImageView();
			imageInfo.sampler = GET_GRAPHICS_SYSTEM()->GetBuffer().GetTextureImageSampler();

			std::array<VkWriteDescriptorSet, 2> descriptorWrites = {};

			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = m_DescriptorSets[i];
			descriptorWrites[0].dstBinding = 0;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pBufferInfo = &bufferInfo;

			descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[1].dstSet = m_DescriptorSets[i];
			descriptorWrites[1].dstBinding = 1;
			descriptorWrites[1].dstArrayElement = 0;
			descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[1].descriptorCount = 1;
			descriptorWrites[1].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
}