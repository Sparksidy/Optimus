#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Descriptors/DescriptorSet.h>
#include <Optimus/Utilities/NonCopyable.h>

namespace OP
{
	class Pipeline;
	class CommandBuffer;
	class UniformHandler;

	class OPTIMUS_API DescriptorHandler: public NonCopyable
	{
	public:
		DescriptorHandler(UniformHandler& uniformHandler, const Pipeline& pipeline);

		explicit DescriptorHandler(const Pipeline& pipeline);

		~DescriptorHandler();

		void BindDescriptor(const CommandBuffer& commandBuffer);

	private:
		std::vector<std::unique_ptr<DescriptorSet>> m_DescriptorSets; //Temporary: Descriptor sets per swapchain image
		std::vector<VkWriteDescriptorSet> m_WriteDescriptorSets;
	};
}