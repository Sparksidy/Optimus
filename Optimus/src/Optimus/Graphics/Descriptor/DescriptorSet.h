#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>


namespace OP
{
	class OPTIMUS_API DescriptorSet
	{
	public:
		DescriptorSet();

		~DescriptorSet();

		inline operator const std::vector<VkDescriptorSet>& ()const { return m_DescriptorSets; }

		const std::vector<VkDescriptorSet>& GetDescriptorSet() const { return m_DescriptorSets; }

	private:
		void _createDescriptorSets();

	private:
		std::vector<VkDescriptorSet> m_DescriptorSets;
	};
}