#include <pch.h>
#include <Optimus/Graphics/Descriptor/DescriptorPool.h>
#include <Optimus/Application.h>
#include <Optimus/Graphics/Graphics.h>
#include <Optimus/Graphics/RenderPass/SwapChain.h>
#include <Optimus/Graphics/Devices/LogicalDevice.h>

namespace OP
{
	DescriptorPool::DescriptorPool()
	{
		_createDescriptorPool();
	}
	DescriptorPool::~DescriptorPool()
	{
		vkDestroyDescriptorPool(GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), m_DescriptorPool, nullptr);
	}
	void DescriptorPool::_createDescriptorPool()
	{
		int swapChainImages = GET_GRAPHICS_SYSTEM()->GetSwapchain().GetSwapChainImages();

		VkDescriptorPoolSize poolSize = {};
		poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSize.descriptorCount = static_cast<uint32_t>(swapChainImages);
		VkDescriptorPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = &poolSize;
		poolInfo.maxSets = static_cast<uint32_t>(swapChainImages);

		OP_VULKAN_ASSERT(vkCreateDescriptorPool, GET_GRAPHICS_SYSTEM()->GetLogicalDevice(), &poolInfo, nullptr, &m_DescriptorPool);
		OP_CORE_INFO("Descriptor Pool is created");
	}
}