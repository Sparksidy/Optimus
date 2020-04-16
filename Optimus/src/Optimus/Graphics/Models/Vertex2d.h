#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/Pipelines/Shader.h>

#include <glm.hpp>

namespace OP
{
	class OPTIMUS_API Vertex2d
	{
	public:
		Vertex2d(const glm::vec2& position, const glm::vec2& texCoords) :
			m_Position(position),
			m_TexCoord(texCoords) {}

		static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0)
		{
			std::vector<VkVertexInputBindingDescription> bindingDescriptions;
			{
				VkVertexInputBindingDescription bindingDescription = {};
				bindingDescription.binding = baseBinding;
				bindingDescription.stride = sizeof(Vertex2d);
				bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			}

			std::vector<VkVertexInputAttributeDescription> attributeDescriptions; 
			{
				//Position
				attributeDescriptions[0].binding = baseBinding;
				attributeDescriptions[0].location = 0;
				attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[0].offset = offsetof(Vertex2d, m_Position);

				//UV
				attributeDescriptions[1].binding = baseBinding;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex2d, m_TexCoord);
			}

			return { bindingDescriptions, attributeDescriptions };
		}

	public:
		glm::vec2 m_Position;
		glm::vec2 m_TexCoord;
	};
}
