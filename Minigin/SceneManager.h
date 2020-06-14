#pragma once
#include <vector>
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

		Scene* CreateScene(const std::string& name, bool isTileMap = false, const std::string& levelDataPath = "");
		void SetActiveScene(std::string& name);
		void SetActiveScene(int index);
		Scene* GetActiveScene()const { return m_pActiveScene; };
		int GetAmountOfScenes();

		void Update();
		void PhysicsUpdate();
		bool ProcessInput();
		void Render();
	private:
		std::vector<Scene*> m_Scenes;
		Scene* m_pActiveScene;
	};