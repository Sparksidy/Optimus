#include <pch.h>
#include <Optimus/Graphics/Subrenders/SubRender2D.h>
#include <Optimus/Graphics/Models/Vertex2d.h>
#include <Optimus/Graphics/Commands/CommandBuffer.h>

#include <Optimus/Application.h>
#include <Optimus/Systems/GameObjectManager.h>
#include <Optimus/Core/GameObject.h>
#include <Optimus/Core/MeshComponent.h>
#include <Optimus/Core/TransformComponent.h>

namespace OP
{
	SubRender2D::SubRender2D(const Pipeline::Stage& stage): SubRender(stage), 
		m_Pipeline(stage, 
			{ 
				"C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_frag.spv",
				"C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Shaders\\SPIR-V\\Generic_vert.spv" 
			}, 
		    { 
				Vertex2d::GetVertexInput() 
			})
	{
		OP_CORE_INFO("Created Subrender");
	}
	void SubRender2D::Render(const CommandBuffer& commandBuffer)
	{
		m_Pipeline.BindPipeline(commandBuffer); //Binds the Gfx Pipeline

		//If window resize then update viewport and scissors dynamic states
		m_Pipeline.UpdateViewportsAndScissors(commandBuffer);

		//Render Game Objects
		std::vector<GameObject*> objs = dynamic_cast<OP::GameObjectManager*>(Application::Get().GetSystem("GameObjectManager"))->GetAllGameObjects();
		for (int i = 0; i < objs.size(); i++)
		{
			TransformComponent* transform = dynamic_cast<TransformComponent*>(objs[i]->GetComponent(TRANSFORM_COMPONENT));
			if (transform)
			{
				MeshComponent* mesh = dynamic_cast<MeshComponent*>(objs[i]->GetComponent(MESH_COMPONENT));
				if (mesh)
				{
					mesh->PushTransform(transform->GetTranslation(), transform->GetRotation(), transform->GetScale());
					mesh->Render(commandBuffer, m_Pipeline);
				}
					
			}
		}
	}
}