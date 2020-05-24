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
	DescriptorHandler::DescriptorHandler()
	{	
	}

	DescriptorHandler::~DescriptorHandler()
	{
	}

	bool DescriptorHandler::Update(UniformHandler& uniform, Image2D& image,  const Pipeline& pipeline)
	{
		if (!m_isInitialized)
		{
			m_DescriptorSet = std::make_unique<DescriptorSet>(pipeline);

			OP_CORE_INFO("Descriptor Set created");
			size_t swapchainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
			for (size_t i = 0; i < swapchainImages; i++)
			{
				//Uniform Buffer
				VkDescriptorBufferInfo bufferInfo{};
				bufferInfo.buffer = uniform.GetUniformBuffer(i)->GetBuffer(); //TODO: Remove this 
				bufferInfo.offset = 0;
				bufferInfo.range = VK_WHOLE_SIZE;

				VkWriteDescriptorSet descriptorWrite{};
				descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite.dstSet = m_DescriptorSet->GetDescriptorSet(i);
				descriptorWrite.dstBinding = 0;
				descriptorWrite.dstArrayElement = 0;
				descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				descriptorWrite.descriptorCount = 1;
				descriptorWrite.pBufferInfo = &bufferInfo;

				m_WriteDescriptorSets.emplace_back(descriptorWrite);

				VkDescriptorImageInfo imageInfo{};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = image.GetView();
				imageInfo.sampler = image.GetSampler();

				VkWriteDescriptorSet descriptorWrite2{};
				descriptorWrite2.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				descriptorWrite2.dstSet = m_DescriptorSet->GetDescriptorSet(i);
				descriptorWrite2.dstBinding = 1;
				descriptorWrite2.dstArrayElement = 0;
				descriptorWrite2.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descriptorWrite2.descriptorCount = 1;
				descriptorWrite2.pImageInfo = &imageInfo;

				m_WriteDescriptorSets.emplace_back(descriptorWrite2);

				m_DescriptorSet->Update(m_WriteDescriptorSets);
			}

			m_isInitialized = true;

			OP_CORE_INFO("Descriptors initialized");
		}

		return true;
	}
	
	void DescriptorHandler::BindDescriptor(const CommandBuffer& commandBuffer)
	{
		m_DescriptorSet->BindDescriptor(commandBuffer);
	}
}