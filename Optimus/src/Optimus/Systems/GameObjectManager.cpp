#include <pch.h>
#include <Optimus/Systems/GameObjectManager.h>
#include <Optimus/Core/GameObject.h>

namespace OP
{
	GameObjectManager::GameObjectManager()
	{
	}

	GameObjectManager::~GameObjectManager()
	{
	}

	void GameObjectManager::Update()
	{
	}

	void GameObjectManager::Unload()
	{
		//Release Game Objects

	}

	GameObject* GameObjectManager::LoadObject(const JSONObject& object)
	{
		GameObject* gameObject = new GameObject();

		auto& componentsMember = object.FindMember("Components");
		if (componentsMember != object.MemberEnd())
		{
			auto componentsObject = componentsMember->value.GetObject();
			for (auto componentsIterator = componentsObject.MemberBegin(); componentsIterator != componentsObject.MemberEnd(); ++componentsIterator)
			{
				char const* componentType = componentsIterator->name.GetString();
				auto componentObject = componentsIterator->value.GetObject();

				if (strcmp(componentType, "Transform") == 0)
				{
					gameObject->AddComponent(TRANSFORM_COMPONENT, componentObject);
				}
				else if (strcmp(componentType, "Sprite") == 0)
				{
					gameObject->AddComponent(MESH_COMPONENT, componentObject);
				}
			}
		}

		m_GameObjects.emplace_back(gameObject);

		return gameObject;
	}

}

