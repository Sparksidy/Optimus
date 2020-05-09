#include <pch.h>
#include <Optimus/Graphics/Descriptors/DescriptorHandler.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Pipelines/Pipeline.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>
#include <Optimus/Graphics/Descriptors/Descriptor.h>

namespace OP
{
	DescriptorHandler::DescriptorHandler(UniformHandler& uniformHandler, const Pipeline& pipeline)
	{
		//Create descriptor set per swapchain image
		size_t swapchainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		m_DescriptorSets.resize(swapchainImages);
		for (size_t i = 0; i < swapchainImages; i++)
		{
			m_DescriptorSets[i] = std::make_unique<DescriptorSet>(pipeline);
		}

		OP_CORE_INFO("Allocated descriptor sets");


		//Create Write Descriptor Sets for each uniform buffer
		for (size_t i = 0; i < swapchainImages; i++)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = uniformHandler.GetUniformBuffer(i)->GetBuffer();
			bufferInfo.offset = 0;
			bufferInfo.range = VK_WHOLE_SIZE;

			VkWriteDescriptorSet descriptorWrite{};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_DescriptorSets[i]->GetDescriptorSet();
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;

			m_WriteDescriptorSets.emplace_back(descriptorWrite);

			m_DescriptorSets[i]->Update(m_WriteDescriptorSets);
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
		m_DescriptorSets[imageIndex]->BindDescriptor(commandBuffer);
	}
}