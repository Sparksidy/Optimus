#pragma once
#include <vulkan/vulkan.hpp>

#include <Optimus/Core.h>

namespace OP
{

	class OPTIMUS_API DescriptorPool
	{
	public:
		DescriptorPool();

		~DescriptorPool();

		inline operator const VkDescriptorPool& ()const { return m_DescriptorPool; }

		const VkDescriptorPool& GetDescriptorPool() const { return m_DescriptorPool; }

	private:
		void _createDescriptorPool();

	private:
		VkDescriptorPool m_DescriptorPool;

	};

}
