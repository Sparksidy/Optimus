#include <pch.h>
#include <Optimus/Graphics/Buffers/UniformBuffer.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

namespace OP
{
	UniformBuffer::UniformBuffer(VkDeviceSize size, const void* data):
		Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, data)
	{
	}

	void UniformBuffer::Update(const void* newData)
	{
		void* data;
		MapMemory(&data);
		std::memcpy(data, newData, static_cast<std::size_t>(m_Size));
		UnmapMemory();
	}

	WriteDescriptorSet UniformBuffer::GetWriteDescriptorSet() const
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = m_Buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = m_Size;

		VkWriteDescriptorSet descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = VK_NULL_HANDLE;
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.pBufferInfo = &bufferInfo;
		return { descriptorWrite, bufferInfo };
	}

	VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderFlags, uint32_t count)
	{
		VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
		descriptorSetLayoutBinding.binding = binding;
		descriptorSetLayoutBinding.descriptorType = descriptorType;
		descriptorSetLayoutBinding.descriptorCount = 1;
		descriptorSetLayoutBinding.stageFlags = shaderFlags;
		descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
		return descriptorSetLayoutBinding;
	}
}


