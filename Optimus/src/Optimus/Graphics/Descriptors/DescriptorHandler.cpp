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

		//Loop through all descriptors
			//Get the right write descriptor set for the resource(Image, Buffer etc)
			//Update m_writeDescriptorSet

		for (size_t i = 0; i < swapchainImages; i++)
		{
			//Uniform Buffer
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
		}

		//Image
		WriteDescriptorSet imageDescriptor = image.GetWriteDescriptorSet(1);
		imageDescriptor.GetWriteDescriptorSet().dstSet = m_DescriptorSets->GetDescriptorSet();
		

		m_WriteDescriptorSets.emplace_back(imageDescriptor.GetWriteDescriptorSet());

		m_DescriptorSets->Update(m_WriteDescriptorSets);

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
		m_DescriptorSets->BindDescriptor(commandBuffer);
	}
}