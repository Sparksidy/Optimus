#include <pch.h>
#include <Optimus/Graphics/Descriptor/DescriptorSetLayout.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>

#include <Optimus/Log.h>

namespace OP
{
	DescriptorSetLayout::DescriptorSetLayout()
	{
		_createDescriptorSetLayout();
	}

	DescriptorSetLayout::~DescriptorSetLayout()
	{
		vkDestroyDescriptorSetLayout(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_descriptorSetLayout, nullptr);

		OP_CORE_INFO("Descriptor Set Layout Freed");
	}

	void DescriptorSetLayout::_createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		OP_VULKAN_ASSERT(vkCreateDescriptorSetLayout,
			GET_GRAPHICS_SYSTEM()->GetLogicalDevice(),
			&layoutInfo,
			nullptr,
			&m_descriptorSetLayout)

		OP_CORE_INFO("Descriptor Set Layout created successfully");
	}
}

