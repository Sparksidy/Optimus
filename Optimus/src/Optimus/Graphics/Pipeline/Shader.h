#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class OPTIMUS_API Shader
	{
	public:
		Shader();
		~Shader();

		void LoadShader(const std::string& path, bool isVert);

		const VkShaderModule& GetVertShaderModule()const { return m_VertShaderModule; }
		const VkShaderModule& GetFragShaderModule()const { return m_FragShaderModule; }

	private:
		std::vector<char> readFile(const std::string& filename);
		VkShaderModule createShaderModule(const std::vector<char>& code);

	private:
		VkShaderModule m_VertShaderModule;
		VkShaderModule m_FragShaderModule;
	};
}