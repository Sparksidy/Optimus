#include <pch.h>
#include <Optimus/Core/Scene.h>
#include <Optimus/Application.h>

#include <Optimus/Systems/GameObjectManager.h>
#include <writer.h>

namespace OP
{
	Scene::Scene(const std::string& sceneData):
		m_Jsonscene()
	{
		OP_CORE_INFO("Constructing Scene");

		m_Jsonscene.Parse(sceneData.c_str());
	}

	Scene::~Scene()
	{
		OP_CORE_INFO("Destructing Scene");
	}

	void Scene::Initialize()
	{
		rapidjson::GenericArray<false, rapidjson::Value>& objsArray = m_Jsonscene["Objects"].GetArray();
		for (auto objectIterator = objsArray.Begin(); objectIterator != objsArray.End(); ++objectIterator)
		{
			//TODO: GameObjects are added per layer
			GetGameObjectManager()->LoadObject(objectIterator->GetObject());
		}
	}
	GameObjectManager* Scene::GetGameObjectManager()
	{ 
		static GameObjectManager* gameObjectManager = nullptr;
		if (!gameObjectManager)
			gameObjectManager = dynamic_cast<OP::GameObjectManager*>(Application::Get().GetSystem("GameObjectManager"));

		return gameObjectManager;
	}
}