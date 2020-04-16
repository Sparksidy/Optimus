#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	/*
		Interface representing Buffer
	*/
	class OPTIMUS_API Buffer
	{
	public:
		Buffer(VkDeviceSize size, VkBufferUsageFlags flags, VkMemoryPropertyFlags memoryFlags, const void* data = nullptr);

		virtual ~Buffer();

		void MapMemory(void** data) const;

		void UnmapMemory()const;

		static uint32_t FindMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& requiredProperties);

		VkDeviceSize GetSize()const { return m_Size; }
		const VkBuffer& GetBuffer()const { return m_Buffer; }
		const VkDeviceMemory& GetDeviceMemory()const { return m_BufferMemory; }

	protected:
		VkDeviceSize m_Size;
		VkBuffer m_Buffer = VK_NULL_HANDLE;
		VkDeviceMemory m_BufferMemory = VK_NULL_HANDLE;
	};

}