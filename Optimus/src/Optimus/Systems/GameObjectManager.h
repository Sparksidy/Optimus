#pragma once

#include <Optimus/Core.h>
#include <Optimus/ISystem.h>
#include <vector>
#include <document.h>

namespace OP
{
	class GameObject;
	class OPTIMUS_API GameObjectManager : public ISystem
	{
	public:

		typedef rapidjson::GenericValue<rapidjson::UTF8<>>::Object JSONObject;

		GameObjectManager();
		~GameObjectManager();


		bool Initialize()
		{
			return true;
		}
		void Update() override;
		void Unload() override;

		std::string GetName()const { return "GameObjectManager"; };

		GameObject* LoadObject(const JSONObject& object);

		std::vector<GameObject*> GetAllGameObjects()const { return m_GameObjects; }

	private:
		std::vector<GameObject*> m_GameObjects;
	};
}