#pragma once

#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

namespace OP
{
	class OPTIMUS_API RenderPass
	{
	public:
		RenderPass();
		~RenderPass();

	private:
		VkRenderPass m_RenderPass{ VK_NULL_HANDLE };

	};
}