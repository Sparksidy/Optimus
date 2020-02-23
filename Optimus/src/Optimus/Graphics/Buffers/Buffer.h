#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

namespace OP
{
	class Vertex
	{
	public:

		glm::vec2 pos;
		glm::vec3 color;
		glm::vec2 texCoord;

		//TODO : Make it for multiple vertices
		 static VkVertexInputBindingDescription& getBindingDescriptions()  {
			VkVertexInputBindingDescription bindingDescription = {};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			return bindingDescription;
		}

		 //TODO : Make it for multiple vertices
		 static std::array<VkVertexInputAttributeDescription, 3> & getAttributeDescriptions() {
			std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions = {};

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

			//TextureCoord
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

			return attributeDescriptions;
		}
	};

	class OPTIMUS_API Buffer
	{
	public:
		Buffer(const std::vector<Vertex>& vertices, const std::vector<uint16_t>& indices);
		~Buffer();

		const VkImageView GetTextureImageView()const { return m_TextureImageView; }
		const VkSampler GetTextureImageSampler()const { return m_TextureSampler; }


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

		void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void createTextureImage();
		void createTextureImageView();
		void createTextureSampler();

		void createVertexBuffer();
		void createIndexBuffer();
		void createUniformBuffer();

		void copyBuffer(VkBuffer& srcBuffer, VkBuffer& dstBuffer, VkDeviceSize& size);
		VkCommandBuffer beginSingleTimeCommands();
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);

	private:

		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
		VkImageView m_TextureImageView;
		VkSampler m_TextureSampler;

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
			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 proj;
		}m_UBO;
	};

}