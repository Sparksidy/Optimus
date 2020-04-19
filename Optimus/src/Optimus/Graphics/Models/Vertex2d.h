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
		Vertex2d() = default;
		Vertex2d(const glm::vec2& position, const glm::vec3& color, const glm::vec2& texCoords) :
			m_Position(position),
			m_Color(color),
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

				//Color
				attributeDescriptions[1].binding = baseBinding;
				attributeDescriptions[1].location = 1;
				attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
				attributeDescriptions[1].offset = offsetof(Vertex2d, m_Color);

				//UV
				attributeDescriptions[2].binding = baseBinding;
				attributeDescriptions[2].location = 2;
				attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
				attributeDescriptions[2].offset = offsetof(Vertex2d, m_TexCoord);
			}

			return { bindingDescriptions, attributeDescriptions };
		}
		
		~Vertex2d() {}

		bool operator==(const Vertex2d& other) const
		{
			return (m_Position == other.m_Position && m_TexCoord == other.m_TexCoord && m_Color == other.m_Color);
		}

		bool operator!=(const Vertex2d& other) const
		{
			return !operator==(other);
		}

	public:
		glm::vec2 m_Position;
		glm::vec3 m_Color;
		glm::vec2 m_TexCoord;
	};
}