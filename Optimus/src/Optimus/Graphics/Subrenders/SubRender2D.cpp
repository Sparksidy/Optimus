#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage) :
		SubRender(stage),
		m_Pipeline(stage, { "C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_frag.spv","C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_vert.spv" }, {Vertex2d::GetVertexInput()}),
		m_Quad(std::make_unique<QuadModel>()),
		m_UniformHandler(),
		m_DescriptorHandler(m_UniformHandler, m_Pipeline)
	{
	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		//Updates the uniform
		m_UniformHandler.Update();

		//Bind the pipeline
		m_Pipeline.BindPipeline(commandBuffer);

		//Bind the descriptors
		m_DescriptorHandler.BindDescriptor(commandBuffer);

		//Render the quad TODO: (Loop through all the meshes in the scene instead of one quad)
		m_Quad->CmdRender(commandBuffer);
	}
}