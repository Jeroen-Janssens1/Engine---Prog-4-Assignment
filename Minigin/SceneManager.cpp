#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void dae::SceneManager::Render()
{
	m_ActiveScene->Render();
}

std::shared_ptr<dae::Scene> dae::SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight)
{
	const auto scene = std::shared_ptr<Scene>(new Scene(name, isTileMap, levelDataPath, windowWidth, windowHeight));
	m_Scenes.push_back(scene);
	return scene;
}

void dae::SceneManager::SetActiveScene(std::string& name)
{
	// TODO: Set as active scene
	for (std::shared_ptr<Scene>& scene : m_Scenes)
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
	if (index < m_Scenes.size())
		m_ActiveScene = m_Scenes[index];
}

int dae::SceneManager::GetAmountOfScenes()
{
	return int(m_Scenes.size());
}
