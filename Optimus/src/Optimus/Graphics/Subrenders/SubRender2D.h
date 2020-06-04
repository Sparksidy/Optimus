#pragma once


#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>

//TODO: Remove this from here
#include <Optimus/Scene.h>
#include <Optimus/Log.h>

namespace OP
{
	class CommandBuffer;

	class OPTIMUS_API SubRender2D : public SubRender
	{
	public:
		explicit SubRender2D(const Pipeline::Stage& stage);

		~SubRender2D()
		{
			OP_CORE_INFO(" Destructing Subrender2D");
		}

		void Render(const CommandBuffer& commandBuffer);

	private:
		GraphicsPipeline m_Pipeline;
	};
}
