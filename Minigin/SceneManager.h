#pragma once
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name, bool isTileMap = false, const std::string& levelDataPath = "", int windowWidth = -1, int windowHeight = -1);

		void SetActiveScene(std::string& name);
		void SetActiveScene(int index);
		int GetAmountOfScenes();

		void Update();
		void Render();
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_ActiveScene;
	};
}
