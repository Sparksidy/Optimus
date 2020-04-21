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
		Vertex2d(const glm::vec2& position, const glm::vec3& color) :
			m_Position(position),
			m_Color(color){}

		static Shader::VertexInput GetVertexInput(uint32_t baseBinding = 0)
		{
			std::vector<VkVertexInputBindingDescription> bindingDescriptions =
			{
				{baseBinding,sizeof(Vertex2d),VK_VERTEX_INPUT_RATE_VERTEX}
			};

			std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {
			{0, baseBinding, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2d, m_Position)},
			{1, baseBinding, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex2d, m_Color)}
			};

			return { bindingDescriptions, attributeDescriptions };
		}
		
		~Vertex2d() {}

		bool operator==(const Vertex2d& other) const
		{
			return (m_Position == other.m_Position && m_Color == other.m_Color);
		}

		bool operator!=(const Vertex2d& other) const
		{
			return !operator==(other);
		}

	public:
		glm::vec2 m_Position;
		glm::vec3 m_Color;

		static const int NUMBER_OF_ATTRIBUTES = 2; //Change this if removing/adding attributes
	};
}