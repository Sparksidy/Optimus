#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>

#include <Optimus/Graphics/Buffers/VertexBuffer.h>

namespace OP
{
	class Vertex;
	class OPTIMUS_API GraphicsPipeline
	{	
	public:
		GraphicsPipeline();

		~GraphicsPipeline();

		operator const VkPipeline &() const 
		{
			return m_GraphicsPipeline; 
		}
	private:

		std::vector<char> readFile(const std::string& filename); //TODO
		VkShaderModule createShaderModule(const std::vector<char>& code); //TODO

	private:

		VkPipelineLayout m_PipelineLayout = {};
		VkPipeline m_GraphicsPipeline = {};
	};
}
