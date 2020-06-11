#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

bool SceneManager::Update()
{
	return m_ActiveScene->Update();
}

void SceneManager::Render()
{
	m_ActiveScene->Render();
}

SceneManager::~SceneManager()
{
	m_ActiveScene = nullptr;
	for (size_t i{}; i < m_Scenes.size(); i++)
	{
		delete m_Scenes[i];
		m_Scenes[i] = nullptr;
	}
	m_Scenes.clear();
}

Scene* SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath)
{
	const auto scene = new Scene(name, isTileMap, levelDataPath);
	m_Scenes.push_back(scene);
	return scene;
}

void SceneManager::SetActiveScene(std::string& name)
{
	// TODO: Set as active scene
	for (auto scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_ActiveScene = scene;
			m_ActiveScene->OnLoad();
			return;
		}
	}
}

void SceneManager::SetActiveScene(int index)
{
	if (size_t(index) < m_Scenes.size())
	{
		m_ActiveScene = m_Scenes[index];
		m_ActiveScene->OnLoad();
	}
}

int SceneManager::GetAmountOfScenes()
{
	return int(m_Scenes.size());
}
