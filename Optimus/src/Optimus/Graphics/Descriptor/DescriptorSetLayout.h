#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <glm.hpp>

namespace OP
{
	class OPTIMUS_API DescriptorSetLayout
	{
	public:
		DescriptorSetLayout();

		~DescriptorSetLayout();

		inline operator const VkDescriptorSetLayout& ()const { return m_descriptorSetLayout; }

		const VkDescriptorSetLayout& GetDescriptorSetLayout() const { return m_descriptorSetLayout; }

	private:
		void _createDescriptorSetLayout();


	private:
		VkDescriptorSetLayout m_descriptorSetLayout;
	};
}

