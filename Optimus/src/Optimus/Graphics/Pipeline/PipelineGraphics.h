#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


namespace OP
{
	class Shader;

	class OPTIMUS_API PipelineGraphics
	{
	public:
		PipelineGraphics();
		~PipelineGraphics();

	private:
		std::unique_ptr<Shader> m_Shader;
		VkPipelineLayout m_PipelineLayout;
	};
}