#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Descriptors/Descriptor.h>
#include <Optimus/Graphics/Descriptors/DescriptorSet.h>
#include <Optimus/Utilities/NonCopyable.h>

namespace OP
{
	class Pipeline;
	class CommandBuffer;
	class Image2D;
	class UniformHandler;

	class OPTIMUS_API DescriptorHandler: public NonCopyable
	{
	public:

		DescriptorHandler();
		~DescriptorHandler();

		bool Update(UniformHandler& uniform, Image2D& image, const Pipeline& pipeline); //TODO: Remove Uniform Handler here use the handler you have in your map

		void BindDescriptor(const CommandBuffer& commandBuffer);

	private:
		std::unique_ptr<DescriptorSet> m_DescriptorSet;
		std::vector<VkWriteDescriptorSet> m_WriteDescriptorSets;
		/*
		NOTE: Boolean to create Descriptor Set and write desc set once. Will change
		in the future where our pipeline might change and we might have to update
		our descriptor sets accordingly
		*/
		bool m_isInitialized = false; 
	};
}