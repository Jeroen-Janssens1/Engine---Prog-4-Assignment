#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"

void SceneManager::Update()
{
	m_pActiveScene->Update();
}

void SceneManager::PhysicsUpdate()
{
	m_pActiveScene->PhysicsUpdate();
}

bool SceneManager::ProcessInput()
{
	return m_pActiveScene->ProcessInput();
}

void SceneManager::Render()
{
	m_pActiveScene->Render();
}

SceneManager::~SceneManager()
{
	m_pActiveScene = nullptr;
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
	for (auto scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_pActiveScene = scene;
			m_pActiveScene->OnLoad();
			return;
		}
	}
}

void SceneManager::SetActiveScene(int index)
{
	if (size_t(index) < m_Scenes.size())
	{
		m_pActiveScene = m_Scenes[index];
		m_pActiveScene->OnLoad();
	}
}

int SceneManager::GetAmountOfScenes()
{
	return int(m_Scenes.size());
}
