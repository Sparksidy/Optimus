#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage): SubRender(stage), m_Pipeline(stage, { "C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_frag.spv","C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_vert.spv" }, { Vertex2d::GetVertexInput() })
	{
		m_Mesh = std::make_unique<Mesh>("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\texture.jpg", m_Pipeline);

		//m_Mesh2 = std::make_unique<Mesh>("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\wall.jpg", m_Pipeline);

	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		
		//m_Mesh2->PreRender(commandBuffer); //Binds the uniform handler


		m_Pipeline.BindPipeline(commandBuffer); //Binds the Gfx Pipeline

		m_Mesh->PreRender(commandBuffer); //Binds the uniform handler

		m_Mesh->Render(commandBuffer); //Binds the descriptor sets
		//m_Mesh2->Render(commandBuffer); //Binds the descriptor sets

	}
}