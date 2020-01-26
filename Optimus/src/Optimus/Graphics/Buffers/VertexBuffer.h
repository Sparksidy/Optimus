#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <glm.hpp>
#include <array>

namespace OP
{
	class Vertex
	{
	public:

		glm::vec2 pos;
		glm::vec3 color;

		//TODO : Make it for multiple vertices
		 static VkVertexInputBindingDescription& getBindingDescriptions()  {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		 //TODO : Make it for multiple vertices
		 static std::array<VkVertexInputAttributeDescription, 2> & getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions = {};

			//Position
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, pos);

			//Color
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, color);

			return attributeDescriptions;
		}
	};

	class OPTIMUS_API VertexBuffer
	{
	public:
		VertexBuffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
		~VertexBuffer();

		inline operator const VkBuffer& () const { return m_vertexBuffer; }

		VkDeviceMemory& GetBufferMemory() { return m_vertexBufferMemory; }
		VkDeviceMemory& GetIndexBufferMemory() { return m_indexBufferMemory; }

		const VkBuffer& GetIndexBuffer()const { return m_indexBuffer; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint16_t>& GetIndices() const { return m_Indices; }

	private:
		uint32_t findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties);

		void createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage,
			const VkMemoryPropertyFlags& memoryProperties,  VkBuffer& vertexBuffer,  VkDeviceMemory& m_vertexBufferMemory);

		void createVertexBuffer();
		void createIndexBuffer();

		void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);



	private:
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;

		std::vector<Vertex> m_Vertices;
		std::vector<uint16_t> m_Indices;
	};

}