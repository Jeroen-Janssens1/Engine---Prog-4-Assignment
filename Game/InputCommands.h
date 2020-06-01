#pragma once
#include "Command.h"
#include "SceneManager.h"
#include "Prefabs\PlayerPrefab.h"
 ////////////////////////////////////
// Only for debugging purposes
class NextSceneCommand : public Command
{
private:
	static int index;
	void NextScene()
	{
		//ServiceLocator::GetAudio().Playsound(0);
		dae::SceneManager::GetInstance().SetActiveScene(index);
		index = (index + 1) % dae::SceneManager::GetInstance().GetAmountOfScenes();
	}

public:
	bool Execute() override { NextScene(); return true; }
};
 // Debugging commands end here
//////////////////////////////////////

class FireCommand : public Command
{
public:
	bool Execute() override { Function(); return true; }
};

class MoveLeftCommand : public Command
{
public:
	MoveLeftCommand(std::shared_ptr<PlayerPrefab>& pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer.lock()->MoveLeft(); return true; }
private:
	std::weak_ptr<PlayerPrefab> m_pPlayer;
};

class MoveRightCommand : public Command
{
public:
	MoveRightCommand(std::shared_ptr<PlayerPrefab>& pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer.lock()->MoveRight(); return true; }
private:
	std::weak_ptr<PlayerPrefab> m_pPlayer;
};

class MoveUpCommand : public Command
{
public:
	MoveUpCommand(std::shared_ptr<PlayerPrefab>& pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer.lock()->MoveUp(); return true; }
private:
	std::weak_ptr<PlayerPrefab> m_pPlayer;
};

class MoveDownCommand : public Command
{
public:
	MoveDownCommand(std::shared_ptr<PlayerPrefab>& pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer.lock()->MoveDown(); return true; }
private:
	std::weak_ptr<PlayerPrefab> m_pPlayer;
};