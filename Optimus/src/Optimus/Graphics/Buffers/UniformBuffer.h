#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Descriptors/Descriptor.h>
#include <Optimus/Graphics/Buffers/Buffer.h>

namespace OP
{
	class OPTIMUS_API UniformBuffer : public Descriptor, public Buffer
	{
	public:

		explicit UniformBuffer(VkDeviceSize size, const void* data = nullptr);

		void Update(const void* newData);

		WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding)const override;

		static VkDescriptorSetLayoutBinding GetDescriptorSetLayoutBinding(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags shaderFlags, uint32_t count);
	};
}