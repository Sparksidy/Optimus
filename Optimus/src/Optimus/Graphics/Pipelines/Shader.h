#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

//TESTING
#include <glm.hpp>


namespace OP
{
	class OPTIMUS_API Shader
	{
	public:
		class VertexInput
		{
		public:
			VertexInput(std::vector<VkVertexInputBindingDescription> binding = {}, std::vector<VkVertexInputAttributeDescription> attributes = {}) :
				m_BindingDescriptions(std::move(binding)),
				m_AttributeDescriptions(std::move(attributes)) {}

			const std::vector<VkVertexInputBindingDescription>& GetBindingDescriptions()const { return m_BindingDescriptions; }
			const std::vector<VkVertexInputAttributeDescription>& GetAttributeDescriptions()const { return m_AttributeDescriptions; }

			bool operator<(const VertexInput& other) const
			{
				return m_BindingDescriptions.front().binding < other.m_BindingDescriptions.front().binding;
			}
		private:
			uint32_t m_Binding = 0;
			std::vector<VkVertexInputBindingDescription> m_BindingDescriptions;
			std::vector<VkVertexInputAttributeDescription> m_AttributeDescriptions;
		};

		Shader() {}
		~Shader() {}

		const std::vector<VkDescriptorSetLayoutBinding>& GetDescriptorSetLayouts()const { return m_DescriptorSetLayouts; }
		const std::vector<VkDescriptorPoolSize>& GetDescriptorPools()const { return m_DescriptorPools; }

		
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		VkShaderStageFlagBits GetShaderStage(const std::filesystem::path& path);

		//TODO: Move to FileSystem class	
		static std::vector<char> Shader::readFile(const std::string& filename);

	private:
		void createDescriptorSetLayoutBindings();
		void createDescriptorPools();

	private:
		std::vector<VkDescriptorSetLayoutBinding> m_DescriptorSetLayouts;
		std::vector<VkDescriptorPoolSize> m_DescriptorPools;
	};
}