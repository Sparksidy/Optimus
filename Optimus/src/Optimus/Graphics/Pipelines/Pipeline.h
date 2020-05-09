#pragma once

#include<vulkan/vulkan.hpp>
#include <Optimus/Core.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

namespace OP
{
	class OPTIMUS_API Pipeline
	{
	public:
					
		using Stage = std::pair<uint32_t, uint32_t>; //(renderpass, subpass)

		Pipeline() = default;

		virtual ~Pipeline() = default;

		void BindPipeline(const CommandBuffer& buffer)const
		{
			vkCmdBindPipeline(buffer, GetPipelineBindPoint(), GetPipeline());
		}

		virtual const VkPipelineBindPoint& GetPipelineBindPoint() const = 0;
		virtual const VkPipeline& GetPipeline() const = 0;
		virtual const VkPipelineLayout& GetPipelineLayout() const = 0;
		virtual const VkDescriptorSetLayout& GetDescriptorSetLayout() const = 0;
		virtual const VkDescriptorPool& GetDescriptorPool()const = 0;

		//TODO: DescriptorSet
	};
}