#pragma once


#include <Optimus/Core.h>
#include <Optimus/Log.h>

//rapidjson
#include <document.h>


#include <string>

namespace OP
{
	/*
		A Scene is a composition of multiple layers
	*/

	class GameObjectManager;
	

	class OPTIMUS_API Scene
	{
	public:
		friend class SceneManager;

		Scene(const std::string& sceneData);

		~Scene();

		void Initialize();

		GameObjectManager* GetGameObjectManager();


	private:
		rapidjson::Document m_Jsonscene;
		bool m_IsInitialized = false;

		//TODO: Layers
	};

}
