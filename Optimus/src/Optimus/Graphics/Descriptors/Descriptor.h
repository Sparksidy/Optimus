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
			m_BufferInfo(std::make_unique<VkDescriptorBufferInfo>(bufferInfo))
		{
			m_WriteDescriptorSet.pBufferInfo = m_BufferInfo.get();
		}

		WriteDescriptorSet(const VkWriteDescriptorSet& writeDescriptorSet, const VkDescriptorImageInfo& imageInfo)
			:m_WriteDescriptorSet(writeDescriptorSet),
			m_ImageInfo(std::make_unique<VkDescriptorImageInfo>(imageInfo))
		{
			m_WriteDescriptorSet.pImageInfo = m_ImageInfo.get();
		}

		VkWriteDescriptorSet& GetWriteDescriptorSet() { return m_WriteDescriptorSet; }
		VkDescriptorImageInfo& GetImageDescInfo() { return *m_ImageInfo; }
		VkDescriptorBufferInfo& GetBufferDescInfo() { return *m_BufferInfo; }

	private:
		VkWriteDescriptorSet m_WriteDescriptorSet;
		std::unique_ptr<VkDescriptorBufferInfo> m_BufferInfo;
		std::unique_ptr<VkDescriptorImageInfo> m_ImageInfo;

	};

	class OPTIMUS_API Descriptor
	{
	public:
		Descriptor() = default;

		virtual ~Descriptor() = default;

		virtual WriteDescriptorSet GetWriteDescriptorSet()const = 0;
	};
}