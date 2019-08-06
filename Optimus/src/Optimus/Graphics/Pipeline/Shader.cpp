#include "pch.h"
#include <Optimus/Graphics/Pipeline/Shader.h>
#include <Optimus/Log.h>
#include <Optimus/Graphics/GraphicsSystem.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>


namespace OP
{
	Shader::Shader()
	{
		LoadShader("Optimus/Optimus/src/Optimus/Graphics/Shaders/SPIR-V/Triangle_vert.spv", true);
		LoadShader("Optimus/Optimus/src/Optimus/Graphics/Shaders/SPIR-V/Triangle_frag.spv", true);
	}

	Shader::~Shader()
	{
		vkDestroyShaderModule(GraphicsSystem::Get()->GetLogicalDevice()->GetLogicalDevice(), m_FragShaderModule, nullptr);
		vkDestroyShaderModule(GraphicsSystem::Get()->GetLogicalDevice()->GetLogicalDevice(), m_VertShaderModule, nullptr);

		OP_CORE_INFO("Shader Modules have been destroyed");
	}

	void Shader::LoadShader(const std::string & path, bool isVert)
	{
		if (isVert)
		{
			auto vertShaderCode = readFile(path);
			m_VertShaderModule = createShaderModule(vertShaderCode);
		}
		else
		{
			auto fragShaderCode = readFile(path);
			m_FragShaderModule = createShaderModule(fragShaderCode);
		}
	}

	VkShaderModule Shader::createShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		OP_VULKAN_ASSERT(vkCreateShaderModule, GraphicsSystem::Get()->GetLogicalDevice()->GetLogicalDevice(), &createInfo, nullptr, &shaderModule);

		OP_CORE_INFO("Shader Modules Created");

		return shaderModule;
	}

	std::vector<char> Shader::readFile(const std::string & filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			OP_CORE_FATAL("failed to open file, {0}", filename);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}

	
}

