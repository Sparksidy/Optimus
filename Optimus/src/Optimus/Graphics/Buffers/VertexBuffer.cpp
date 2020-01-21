#include <pch.h>
#include <Optimus/Graphics/Buffers/VertexBuffer.h>

#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Log.h>

namespace OP
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices):m_Vertices(std::move(vertices))
	{
		Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));
		m_size = sizeof(vertices[0]) * vertices.size();

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = m_size;
		bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		OP_VULKAN_ASSERT(vkCreateBuffer, graphics->GetLogicalDevice(), &bufferInfo, nullptr, &m_vertexBuffer);

		OP_INFO("Vertex Buffer successfully created");


		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(graphics->GetLogicalDevice(), m_vertexBuffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		OP_VULKAN_ASSERT(vkAllocateMemory, graphics->GetLogicalDevice(), &allocInfo, nullptr, &m_vertexBufferMemory);
		OP_INFO("Vertex Buffer Memory successfully allocated");

		OP_VULKAN_ASSERT(vkBindBufferMemory, graphics->GetLogicalDevice(), m_vertexBuffer, m_vertexBufferMemory, 0);
		OP_INFO("Bind the buffer memory to the buffer successful");

		void* data;
		MapMemory(&data);
		std::memcpy(data, vertices.data(), (size_t)m_size);
		UnmapMemory();
	}

	VertexBuffer::~VertexBuffer()
	{
		//Cleanup the vertex Buffer
		vkDestroyBuffer(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBuffer, nullptr);
		OP_INFO("Vertex Buffer Destroyed");

		vkFreeMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
		OP_INFO("Buffer Memory Freed");
	}

	void VertexBuffer::MapMemory(void** data) const
	{
		OP_VULKAN_ASSERT(vkMapMemory, dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBufferMemory, 0, m_size, 0, data);
	}

	void VertexBuffer::UnmapMemory() const
	{
		vkUnmapMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBufferMemory);
	}

	uint32_t VertexBuffer::findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties)
	{
		auto memoryProperties = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetPhysicalDevice().GetMemoryProperties();

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
			uint32_t memoryTypeBits = 1 << i;

			if (typeFilter & memoryTypeBits && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
				return i;
			}
		}

		throw std::runtime_error("Failed to find a valid memory type for buffer");
	}

}