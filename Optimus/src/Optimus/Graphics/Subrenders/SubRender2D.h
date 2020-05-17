#pragma once


#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>
#include <Optimus/Graphics/Models/QuadModel.h>
#include <Optimus/Graphics/Buffers/UniformHandler.h>
#include <Optimus/Graphics/Images/Image2D.h>

namespace OP
{
	class CommandBuffer;
	class DescriptorHandler;


	class OPTIMUS_API SubRender2D : public SubRender
	{
	public:
		explicit SubRender2D(const Pipeline::Stage& stage);

		void Render(const CommandBuffer& commandBuffer);

	private:
		GraphicsPipeline m_Pipeline;
		std::unique_ptr<QuadModel> m_Quad;

		UniformHandler m_UniformHandler;
		std::unique_ptr <Image2D> m_Image;
		DescriptorHandler m_DescriptorHandler;
	};

}
