#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

#include <Optimus/Mesh.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage): SubRender(stage), m_Pipeline(stage, { "C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_frag.spv","C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_vert.spv" }, { Vertex2d::GetVertexInput() })
	{
		OP_CORE_INFO("Created Subrender");
	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		m_Pipeline.BindPipeline(commandBuffer); //Binds the Gfx Pipeline

		//If window resize then update viewport and scissors dynamic states
		m_Pipeline.UpdateViewportsAndScissors(commandBuffer);

		//TODO
			//Get the transforms for the meshes
			//Update the uniforms

		size_t numberOfMeshes = dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->GetNumberOfMeshes();

		for(int i = 0; i < numberOfMeshes; i++)
		{
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->GetMesh(i)->Render(commandBuffer, m_Pipeline);
		}
	}
}