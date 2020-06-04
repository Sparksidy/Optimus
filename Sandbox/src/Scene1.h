#pragma once
#include <Optimus/Scene.h>
#include <Optimus/Log.h>

namespace OP
{
	class Scene1 : public Scene
	{
	public:
		Scene1()
		{
			//Make camera for this scene here
		}

		bool Initialize() override
		{
			//Init the scene here, throw the meshes here
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->CreateMeshEntity("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\texture.jpg");
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->CreateMeshEntity("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\wall.jpg");
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->CreateAnimatedMeshEntity("C:\\Users\\sidys\\OneDrive\\Desktop\\Optimus\\Optimus\\src\\Optimus\\Graphics\\Textures\\character.png", 1.0f, 8.0f, 1.0f);

			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->PushTransform(glm::vec3(100.0f, 100.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f), 0);
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->PushTransform(glm::vec3(200.0f, 200.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f), 1);
			dynamic_cast<OP::Scene*>(Application::Get().GetSystem("Scene"))->PushTransform(glm::vec3(540.0f, 360.0f, 0.0f), glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(10.0f, 10.0f, 1.0f), 2);

			return true;
		}

		virtual ~Scene1()
		{
			OP_CORE_INFO("Destroying Scene1");
		}
	};

}