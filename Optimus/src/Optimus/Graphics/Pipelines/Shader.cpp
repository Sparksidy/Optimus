#include "pch.h"
#include <Optimus/Graphics/Pipelines/Shader.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>

namespace OP
{
	Shader::Shader()
	{
		createDescriptorSetLayoutBindings();
		createDescriptorPools();
	}
	Shader::~Shader()
	{
	}
	VkShaderModule Shader::CreateShaderModule(const std::vector<char>& code)
	{
		VkShaderModuleCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		createInfo.codeSize = code.size();
		createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

		VkShaderModule shaderModule;
		if (vkCreateShaderModule(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
			throw std::runtime_error("failed to create shader module!");
		}

		return shaderModule;
	}

	VkShaderStageFlagBits Shader::GetShaderStage(const std::filesystem::path& path)
	{
		auto fileName = path.filename();

		//TODO remove hardcoding
		if (fileName == "Generic_vert.spv")
			return VK_SHADER_STAGE_VERTEX_BIT;
		if (fileName == "Generic_frag.spv")
			return VK_SHADER_STAGE_FRAGMENT_BIT;

		return VK_SHADER_STAGE_ALL;
	}

	void Shader::createDescriptorSetLayoutBindings()
	{
		//TODO: Add this to reflection system to make it dynamic
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		m_DescriptorSetLayouts.emplace_back(uboLayoutBinding);

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		m_DescriptorSetLayouts.emplace_back(samplerLayoutBinding);
	}

	void Shader::createDescriptorPools()
	{
		int swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		//TODO: Add this to reflection system to make it dynamic
		VkDescriptorPoolSize pool_1;
		pool_1.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		pool_1.descriptorCount = static_cast<uint32_t>(swapChainImages);

		m_DescriptorPools.emplace_back(pool_1);

		VkDescriptorPoolSize pool_2;
		pool_2.type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		pool_2.descriptorCount = static_cast<uint32_t>(swapChainImages);

		m_DescriptorPools.emplace_back(pool_2);
	}

	std::vector<char> Shader::readFile(const std::string& filename)
	{
		std::ifstream file(filename, std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}


}

