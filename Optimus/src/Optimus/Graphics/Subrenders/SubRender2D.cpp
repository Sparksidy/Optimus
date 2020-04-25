#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage) :
		SubRender(stage),
		m_Pipeline(stage, { "C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Triangle_frag.spv","C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Triangle_vert.spv" }, {Vertex2d::GetVertexInput()}),
		m_Quad(std::make_unique<QuadModel>())
	{
	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		//Bind the pipeline
		m_Pipeline.BindPipeline(commandBuffer);

		//Render the quad
		m_Quad->CmdRender(commandBuffer);
	}
}