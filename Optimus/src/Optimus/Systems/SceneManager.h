#pragma once

#include <Optimus/Core.h>
#include <Optimus/ISystem.h>
#include <Optimus/Log.h>

#include <unordered_map>
#include <filesystem>
namespace OP
{
	class Scene;
	class OPTIMUS_API SceneManager  : public ISystem
	{
	public:
		SceneManager();

		~SceneManager();

		bool Initialize() override;

		void Update() override;

		void Unload() override;

		bool LoadScenes(const std::filesystem::path& path);

		Scene* LoadScene(const std::string& path);

		void PrepareScene(Scene* scene);

		inline std::string GetName()const { return "SceneManager"; }

	protected:
		std::unordered_map<std::string, std::pair<std::string, Scene*>> m_scenes;

		std::pair<std::string, Scene*>*  m_activeScene;
	};

}
