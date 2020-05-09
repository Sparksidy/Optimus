#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class OPTIMUS_API WriteDescriptorSet
	{
	public:
		WriteDescriptorSet(const VkWriteDescriptorSet& writeDescriptorSet, const VkDescriptorBufferInfo& bufferInfo)
			:m_WriteDescriptorSet(writeDescriptorSet), 
			m_BufferInfo(std::make_unique<VkDescriptorBufferInfo>())
		{
			m_WriteDescriptorSet.pBufferInfo = m_BufferInfo.get();
		}

		const VkWriteDescriptorSet& GetWriteDescriptorSet()const { return m_WriteDescriptorSet; }

	private:
		VkWriteDescriptorSet m_WriteDescriptorSet;
		std::unique_ptr<VkDescriptorBufferInfo> m_BufferInfo;
	};

	class OPTIMUS_API Descriptor
	{
	public:
		Descriptor() = default;

		virtual ~Descriptor() = default;

		virtual WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding)const = 0;
	};
}