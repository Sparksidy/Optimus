#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage): SubRender(stage), m_Pipeline(stage, { "C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_frag.spv","C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_vert.spv" }, { Vertex2d::GetVertexInput() })
	{
		m_Mesh = std::make_unique<Mesh>("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\texture.jpg");
		m_Mesh2 = std::make_unique<Mesh>("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\wall.jpg");
		m_Mesh3 = std::make_unique<AnimatedMesh>("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\character.png", 1.0f, 8.0f, 1.0f);


		m_Mesh->PushTransform(glm::vec3(100.0f, 100.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		m_Mesh2->PushTransform(glm::vec3(200.0f, 200.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));
		m_Mesh3->PushTransform(glm::vec3(540.0f, 360.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f));

	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		m_Pipeline.BindPipeline(commandBuffer); //Binds the Gfx Pipeline

		//Get the transforms for the meshes
		//Update the uniforms
		//Then Render
		m_Mesh->Render(commandBuffer, m_Pipeline);
		m_Mesh2->Render(commandBuffer, m_Pipeline);
		m_Mesh3->Render(commandBuffer, m_Pipeline);

	}
}