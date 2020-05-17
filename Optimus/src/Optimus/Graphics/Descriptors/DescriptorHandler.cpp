#include <pch.h>
#include <Optimus/Graphics/Descriptors/DescriptorHandler.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Pipelines/Pipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Descriptors/Descriptor.h>
#include <Optimus/Graphics/Images/Image2D.h>

namespace OP
{
	DescriptorHandler::DescriptorHandler(const Image2D& image, UniformHandler& uniformHandler, const Pipeline& pipeline)
		:m_DescriptorSets(std::make_unique<DescriptorSet>(pipeline))
	{
		OP_CORE_INFO("Allocated descriptor sets");
		size_t swapchainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		//Bind the Buffer, Image & Sampler Info to the descriptor sets
		//Create Write Descriptor Sets for each Uniform Buffer and Image Sampler
		for (size_t i = 0; i < swapchainImages; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformHandler.GetUniformBuffer(i)->GetBuffer(); //TODO: Remove this 
			bufferInfo.offset = 0;
			bufferInfo.range = VK_WHOLE_SIZE;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets->GetDescriptorSet();
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			m_WriteDescriptorSets.emplace_back(descriptorWrite);

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = image.GetView(); //TODO: Remove this 
			imageInfo.sampler = image.GetSampler();//TODO: Remove this 

			VkWriteDescriptorSet descriptorWrite2{};
			descriptorWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite2.dstSet = m_DescriptorSets->GetDescriptorSet();
			descriptorWrite2.dstBinding = 1;
			descriptorWrite2.dstArrayElement = 0;
			descriptorWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite2.descriptorCount = 1;
			descriptorWrite2.pImageInfo = &imageInfo;

			m_WriteDescriptorSets.emplace_back(descriptorWrite2);

			m_DescriptorSets->Update(m_WriteDescriptorSets);
		}

		OP_CORE_INFO("Created Write Descriptor Sets");

	}
	DescriptorHandler::DescriptorHandler(const Pipeline& pipeline)
	{
	}
	DescriptorHandler::~DescriptorHandler()
	{
	}
	
	void DescriptorHandler::BindDescriptor(const CommandBuffer& commandBuffer)
	{
		uint32_t imageIndex = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetActiveImageIndex();
		m_DescriptorSets->BindDescriptor(commandBuffer);
	}
}