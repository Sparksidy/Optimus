#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#define GLM_FORCE_RADIANS
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

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

	class OPTIMUS_API Buffer
	{
	public:
		Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
		~Buffer();

		inline operator const VkBuffer& () const { return m_vertexBuffer; }
		const VkBuffer& GetIndexBuffer()const { return m_indexBuffer; }
		const std::vector<VkBuffer>& GetUniformBuffer()const { return m_uniformBuffer; }
	

		VkDeviceMemory& GetBufferMemory()  { return m_vertexBufferMemory; }
		VkDeviceMemory& GetIndexBufferMemory() { return m_indexBufferMemory; }
		std::vector<VkDeviceMemory>& GetUniformBufferMemory() { return m_uniformBufferMemory; }

		const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
		const std::vector<uint16_t>& GetIndices() const { return m_Indices; }

		void FreeAndDestroyUniformBuffers();
		void RecreateUniformBuffers();
		void UpdateUniformBuffers(uint32_t currentImage);

		VkDeviceSize GetSizeOfUBO()const { return sizeof(UniformBufferObject); }

	private:
		uint32_t findMemoryType(uint32_t typeFilter, const VkMemoryPropertyFlags& properties);

		void createBuffer(const VkDeviceSize& size, const VkBufferUsageFlags& usage,
			const VkMemoryPropertyFlags& memoryProperties,  VkBuffer& vertexBuffer,  VkDeviceMemory& m_vertexBufferMemory);

		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffer();

		void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);

	private:

		std::vector<Vertex> m_Vertices;
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;

		std::vector<uint16_t> m_Indices;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;

		std::vector<VkBuffer> m_uniformBuffer;
		std::vector<VkDeviceMemory> m_uniformBufferMemory;

		struct UniformBufferObject
		{
			glm::mat4 proj;
			glm::mat4 view;
			glm::mat4 model;
		}m_UBO;
	};

}