#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final
	{
	public:
		SceneManager() = default;
		~SceneManager();
		
		SceneManager(const SceneManager& other) = delete;
		SceneManager(SceneManager&& other) = delete;
		SceneManager& operator=(const SceneManager& other) = delete;
		SceneManager& operator=(SceneManager&& other) = delete;

		Scene* CreateScene(const std::string& name, bool isTileMap = false, const std::string& levelDataPath = "", int windowWidth = -1, int windowHeight = -1);

		void SetActiveScene(std::string& name);
		void SetActiveScene(int index);
		int GetAmountOfScenes();

		void Update();
		void Render();
	private:
		std::vector<Scene*> m_Scenes;
		Scene* m_ActiveScene;
	};
}
