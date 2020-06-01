#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	m_ActiveScene->Update();
}

void dae::SceneManager::Render()
{
	m_ActiveScene->Render();
}

dae::Scene* dae::SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight)
{
	const auto scene = new Scene(name, isTileMap, levelDataPath, windowWidth, windowHeight);
	m_Scenes.push_back(scene);
	return scene;
}

void dae::SceneManager::SetActiveScene(std::string& name)
{
	// TODO: Set as active scene
	for (auto scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_ActiveScene = scene;
			return;
		}
	}
}

void dae::SceneManager::SetActiveScene(int index)
{
	if (size_t(index) < m_Scenes.size())
		m_ActiveScene = m_Scenes[index];
}

int dae::SceneManager::GetAmountOfScenes()
{
	return int(m_Scenes.size());
}
