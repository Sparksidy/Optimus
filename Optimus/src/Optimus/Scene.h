#pragma once

#include <Optimus/Core.h>
#include <Optimus/Mesh.h>
#include <Optimus/AnimatedMesh.h>

namespace OP
{
	class OPTIMUS_API Scene  : public ISystem
	{
	public:
		Scene()
		{
		}

		bool Initialize() override 
		{ 
			return true; 
		}

		inline std::string GetName()const { return "Scene"; }

		void Update() override
		{
			if (!m_scene->m_Started)
			{
				m_scene->Initialize();
				m_scene->m_Started = true;
			}
		}

		void Unload() override {}

		
		Mesh* GetMesh(uint32_t index)const
		{
			if (m_Meshes.empty() || m_Meshes.size() < index)
				return nullptr;

			return m_Meshes.at(index).get();
		}

		size_t GetNumberOfMeshes()const
		{
			return m_Meshes.size();
		}

		void CreateMeshEntity(const char* filename)
		{
			//Init the scene here, throw the meshes here 
			m_Meshes.emplace_back(std::move(std::make_unique<Mesh>(filename)));
		}

		void CreateAnimatedMeshEntity(const char* filename, float framesPerElement, float frames, float duration)
		{
			//Init the scene here, throw the meshes here 
			m_Meshes.emplace_back(std::move(std::make_unique<AnimatedMesh>(filename, framesPerElement, frames, duration)));
		}

		void PushTransform(glm::vec3 translate, float angle, glm::vec3 rotateaxis, glm::vec3 scale, int index)
		{
			m_Meshes[index]->PushTransform(translate, angle, rotateaxis, scale);
		}

		void SetScene(std::unique_ptr<Scene>&& scene)
		{ 
			m_scene = std::move(scene); 
		}

		virtual ~Scene()
		{
			OP_CORE_INFO("Destroying Scene");
		}

	protected:
		std::vector<std::unique_ptr<Mesh>> m_Meshes; //TODO: Change to entitites. Currently Testing
		std::unique_ptr<Scene> m_scene;

		bool m_Started = false;
	};

}
