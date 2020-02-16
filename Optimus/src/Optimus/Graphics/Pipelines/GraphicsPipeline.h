#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>


namespace OP
{
	class OPTIMUS_API GraphicsPipeline
	{	
	public:
		GraphicsPipeline();

		~GraphicsPipeline();

		operator const VkPipeline &() const 
		{
			return m_GraphicsPipeline; 
		}

		const VkPipelineLayout& GetPipelineLayout()const { return m_PipelineLayout; }
	private:

		std::vector<char> readFile(const std::string& filename); //TODO
		VkShaderModule createShaderModule(const std::vector<char>& code); //TODO

	private:

		VkPipelineLayout m_PipelineLayout = {};
		VkPipeline m_GraphicsPipeline = {};
	};
}
