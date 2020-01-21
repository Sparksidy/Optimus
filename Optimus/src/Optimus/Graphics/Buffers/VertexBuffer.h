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
		VertexBuffer(const std::vector<Vertex>& vertices);
		~VertexBuffer();

		inline operator const VkBuffer& () const { return m_vertexBuffer; }

		VkDeviceMemory& GetBufferMemory() { return m_vertexBufferMemory; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }

		void MapMemory(void** data) const;

		void UnmapMemory() const;

	private:
		uint32_t findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties);

	private:
		VkDeviceSize m_size;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		std::vector<Vertex> m_Vertices;
	};
}