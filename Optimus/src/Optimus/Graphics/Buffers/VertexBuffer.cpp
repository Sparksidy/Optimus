#include <pch.h>
#include <Optimus/Graphics/Buffers/VertexBuffer.h>

#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Log.h>

namespace OP
{
	VertexBuffer::VertexBuffer(const std::vector<Vertex>& vertices):m_Vertices(std::move(vertices))
	{
		createVertexBuffer();
	}

	VertexBuffer::~VertexBuffer()
	{
		//Cleanup the vertex Buffer
		vkDestroyBuffer(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBuffer, nullptr);
		OP_INFO("Vertex Buffer Destroyed");

		vkFreeMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
		OP_INFO("Buffer Memory Freed");
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

	void VertexBuffer::createBuffer(const VkDeviceSize& size, 
									const VkBufferUsageFlags& usage,
									const VkMemoryPropertyFlags& memoryProperties, 
									VkBuffer& vertexBuffer, 
									VkDeviceMemory& vertexBufferMemory)
	{
		Graphics* graphics = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"));

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		OP_VULKAN_ASSERT(vkCreateBuffer, graphics->GetLogicalDevice(), &bufferInfo, nullptr, &vertexBuffer);
		OP_INFO("Vertex Buffer successfully created");

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(graphics->GetLogicalDevice(), vertexBuffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

		OP_VULKAN_ASSERT(vkAllocateMemory, graphics->GetLogicalDevice(), &allocInfo, nullptr, &vertexBufferMemory);
		OP_INFO("Vertex Buffer Memory successfully allocated");

		OP_VULKAN_ASSERT(vkBindBufferMemory, graphics->GetLogicalDevice(), vertexBuffer, vertexBufferMemory, 0);
		OP_INFO("Bind the buffer memory to the buffer successful");
	}

	void VertexBuffer::createVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		OP_VULKAN_ASSERT(vkMapMemory, dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		std::memcpy(data, m_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

		copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

		//Clean up Staging buffer
		vkDestroyBuffer(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void VertexBuffer::copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
	{	
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion = {};
		copyRegion.srcOffset = 0; // Optional
		copyRegion.dstOffset = 0; // Optional
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice().GetGraphicsQueue());

		//TODO
		vkFreeCommandBuffers(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetCommandPool(), 1, &commandBuffer);
	}
}