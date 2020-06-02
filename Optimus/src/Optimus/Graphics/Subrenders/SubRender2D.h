#pragma once


#include <vulkan/vulkan.hpp>
#include <Optimus/Core.h>

#include <Optimus/Graphics/Core/SubRender.h>
#include <Optimus/Graphics/Pipelines/GraphicsPipeline.h>

//TODO: Remove this from here
#include <Optimus/Mesh.h>
#include <Optimus/AnimatedMesh.h>

namespace OP
{
	class CommandBuffer;

	class OPTIMUS_API SubRender2D : public SubRender
	{
	public:
		explicit SubRender2D(const Pipeline::Stage& stage);

		void Render(const CommandBuffer& commandBuffer);

	private:
		GraphicsPipeline m_Pipeline;
		std::unique_ptr<Mesh> m_Mesh; //Temporary: Fix me by adding Scene 
		std::unique_ptr<Mesh> m_Mesh2; //Temporary: Fix me by adding Scene 
		std::unique_ptr<AnimatedMesh> m_Mesh3; //Temporary: Fix me by adding Scene 

	};
}
