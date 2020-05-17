#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Descriptors/DescriptorSet.h>
#include <Optimus/Utilities/NonCopyable.h>

namespace OP
{
	class Pipeline;
	class CommandBuffer;
	class UniformHandler; //TODO: Remove this 
	class Image2D;		//TODO: Remove this 

	class OPTIMUS_API DescriptorHandler: public NonCopyable
	{
	public:
		//TODO: Remove this dependency of Image and Uniform for the descriptor Handler
		DescriptorHandler(const Image2D& image, UniformHandler& uniformHandler, const Pipeline& pipeline);

		explicit DescriptorHandler(const Pipeline& pipeline);

		~DescriptorHandler();

		void BindDescriptor(const CommandBuffer& commandBuffer);

	private:
		std::unique_ptr<DescriptorSet> m_DescriptorSets;
		std::vector<VkWriteDescriptorSet> m_WriteDescriptorSets;
	};
}