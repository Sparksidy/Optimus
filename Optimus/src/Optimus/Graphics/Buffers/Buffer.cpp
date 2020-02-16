#include <pch.h>

#include <chrono>
#include <Optimus/Graphics/Buffers/Buffer.h>

#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/Devices/PhysicalDevice.h>
#include <Optimus/Graphics/Commands/CommandPool.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Log.h>

namespace OP
{
	Buffer::Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices):
		m_Vertices(std::move(vertices)),
		m_Indices(std::move(indices))
	{
		createVertexBuffer();
		createIndexBuffer();
		createUniformBuffer();
	}

	Buffer::~Buffer()
	{
		//Cleanup the vertex Buffer
		vkDestroyBuffer(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBuffer, nullptr);
		OP_INFO("Vertex Buffer Destroyed");

		vkFreeMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_vertexBufferMemory, nullptr);
		OP_INFO("Vertex Buffer Memory Freed");

		vkDestroyBuffer(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_indexBuffer, nullptr);
		OP_INFO("Index Buffer Destroyed");

		vkFreeMemory(dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetLogicalDevice(), m_indexBufferMemory, nullptr);
		OP_INFO("Index Buffer Memory Freed");
	}

	void Buffer::FreeAndDestroyUniformBuffers()
	{
		int size = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();
		for (size_t i = 0; i < size; i++)
		{
			vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBuffer[i], nullptr);
			vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(),m_uniformBufferMemory[i], nullptr);
		}
	}

	void Buffer::RecreateUniformBuffers()
	{
		//createUniformBuffer();
	}

	void Buffer::UpdateUniformBuffers(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

		ubo.proj = glm::perspective(glm::radians(45.0f), GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().width / (float)GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainExtent().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBufferMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_uniformBufferMemory[currentImage]);
	}

	uint32_t Buffer::findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties)
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

	void Buffer::createBuffer(const VkDeviceSize& size, 
									const VkBufferUsageFlags& usage,
									const VkMemoryPropertyFlags& memoryProperties, 
									VkBuffer& Buffer, 
									VkDeviceMemory& BufferMemory)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		OP_VULKAN_ASSERT(vkCreateBuffer, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &bufferInfo, nullptr, &Buffer);
		OP_INFO("Buffer successfully created");

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), Buffer, &memoryRequirements);

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memoryRequirements.size;
		allocInfo.memoryTypeIndex = findMemoryType(memoryRequirements.memoryTypeBits, memoryProperties);

		OP_VULKAN_ASSERT(vkAllocateMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, nullptr, &BufferMemory);
		OP_INFO("Buffer Memory successfully allocated");

		OP_VULKAN_ASSERT(vkBindBufferMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), Buffer, BufferMemory, 0);
		OP_INFO("Buffer memory binded to the buffer");
	}

	void Buffer::createVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(m_Vertices[0]) * m_Vertices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		OP_VULKAN_ASSERT(vkMapMemory, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		std::memcpy(data, m_Vertices.data(), (size_t)bufferSize);
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_vertexBuffer, m_vertexBufferMemory);

		copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);

		//Clean up Staging buffer
		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void Buffer::createIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(m_Indices[0]) * m_Indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, m_Indices.data(), (size_t)bufferSize);
		vkUnmapMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_indexBuffer, m_indexBufferMemory);

		copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);

		vkDestroyBuffer(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBuffer, nullptr);
		vkFreeMemory(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), stagingBufferMemory, nullptr);
	}

	void Buffer::createUniformBuffer()
	{
		//Create uniform buffers per swapchain image
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);
		int swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		m_uniformBuffer.resize(swapChainImages);
		m_uniformBufferMemory.resize(swapChainImages);

		for (size_t i = 0; i < swapChainImages; i++)
		{
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffer[i], m_uniformBufferMemory[i]);
		}

		OP_CORE_INFO("Uniform Buffers Created");
	}

	void Buffer::copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size)
	{	
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = GET_GRAPHICS_SYSTEM()->GetCommandPool();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &allocInfo, &commandBuffer);

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

		vkQueueSubmit(GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(GET_GRAPHICS_SYSTEM()->GetLogicalDevice().GetGraphicsQueue());

		vkFreeCommandBuffers(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), dynamic_cast<Graphics*>(Application::Get().GetSystem("Graphics"))->GetCommandPool(), 1, &commandBuffer);
	}

}