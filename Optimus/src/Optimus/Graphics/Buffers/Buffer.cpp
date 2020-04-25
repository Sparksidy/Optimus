#include <pch.h>

#include <Optimus/Graphics/Buffers/Buffer.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Log.h>

namespace OP
{
	Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryProperty, const void* data):
		m_Size(size)
	{
		//Create buffer handle
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_Size;
		bufferInfo.usage = flags;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		OP_VULKAN_ASSERT(vkCreateBuffer, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &bufferInfo, nullptr, &m_Buffer);
	
		//Allocate memory for the buffer
		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Buffer, &memoryRequirements);
		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = FindMemoryType(memoryRequirements.memoryTypeBits, memoryProperty);
		OP_VULKAN_ASSERT(vkAllocateMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, nullptr, &m_BufferMemory);

		if (data)
		{
			void* mapped;
			MapMemory(&mapped);

			std::memcpy(mapped, data, m_Size);

			// If host coherency hasn't been requested, do a manual flush to make writes visible.
			if ((memoryProperty & VK_MEMORY_PROPERTY_HOST_COHERENT_BIT) == 0)
			{
				VkMappedMemoryRange mappedMemoryRange = {};
				mappedMemoryRange.sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE;
				mappedMemoryRange.memory = m_BufferMemory;
				mappedMemoryRange.offset = 0;
				mappedMemoryRange.size = m_Size;
				vkFlushMappedMemoryRanges(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), 1, &mappedMemoryRange);
			}

			UnmapMemory();
		}

		//Bind the memory to the buffer object
		OP_VULKAN_ASSERT(vkBindBufferMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Buffer, m_BufferMemory, 0);
	}
	Buffer::~Buffer()
	{
		OP_CORE_TRACE("Destroying Buffers");
		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_Buffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_BufferMemory, nullptr);
	}
	void Buffer::MapMemory(void** data) const
	{
		OP_VULKAN_ASSERT(vkMapMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_BufferMemory, 0, m_Size, 0, data);
	}
	void Buffer::UnmapMemory() const
	{
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_BufferMemory);
	}
	uint32_t Buffer::FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& requiredProperties)
	{
		auto memoryProperties = GET_GRAPHICS_SYSTEM()->GetPhysicalDevice().GetMemoryProperties();

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
		{
			uint32_t memoryTypeBits = 1 << i;

			if (typeFilter & memoryTypeBits && (memoryProperties.memoryTypes[i].propertyFlags & requiredProperties) == requiredProperties)
			{
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer");
	}
}