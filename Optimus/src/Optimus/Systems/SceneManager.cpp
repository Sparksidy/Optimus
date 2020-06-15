#include <pch.h>
#include <Optimus/Systems/SceneManager.h>
#include <Optimus/Core/Scene.h>

#include <iostream>
#include <string>

namespace OP
{
	SceneManager::SceneManager()
	{
		OP_CORE_INFO("Constructing SceneManager");
	}

	SceneManager:: ~SceneManager()
	{
		OP_CORE_INFO("Destructing SceneManager");
	}
	
	bool SceneManager::Initialize()
	{
		return true;
	}

	void SceneManager::Update()
	{
	}

	void SceneManager::Unload()
	{
	}

	bool SceneManager::LoadScenes(const std::filesystem::path& path)
	{
		std::vector<std::pair<std::string, std::string>> scenes;
		std::filesystem::directory_iterator end;
		for (std::filesystem::directory_iterator entry(path); entry != end; entry++)
		{
			if (entry->status().type() == std::filesystem::file_type::regular)
			{
				auto path = entry->path();
				if (path.extension().string() == ".json")
					scenes.push_back(std::make_pair(path.stem().string(), path.string()));
			}
		}

		for (auto scenePair : scenes)
		{
			Scene* scene = LoadScene(scenePair.second);

			if (scene)
				m_scenes[scenePair.first] = std::make_pair(scenePair.second, scene);
		}

		//Get the first scene and make it active
		std::string const& activeSceneName = scenes[0].first;
		if (m_scenes.count(activeSceneName))
		{
			m_activeScene = &m_scenes[activeSceneName];
			PrepareScene((*m_activeScene).second);
			return true;
		}

		return false;
	}

	Scene* SceneManager::LoadScene(const std::string& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);

		if (!file.is_open())
		{
			OP_CORE_ERROR("File is open {0}", path);
			return nullptr;
		}

		std::string levelData((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		
		return new Scene(levelData);
	}

	void SceneManager::PrepareScene(Scene* scene)
	{
		if (!scene->m_IsInitialized)
			scene->Initialize();
	}

}