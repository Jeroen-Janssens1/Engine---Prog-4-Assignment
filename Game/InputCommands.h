#pragma once
#include "Command.h"
#include "SceneManager.h"
#include "SoundManager.h"
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
		ServiceLocator<dae::SceneManager, dae::SceneManager>::GetService().SetActiveScene(index);
		index = (index + 1) % ServiceLocator<dae::SceneManager, dae::SceneManager>::GetService().GetAmountOfScenes();
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
	MoveLeftCommand(PlayerPrefab* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveLeft(); return true; }
private:
	PlayerPrefab* m_pPlayer;
};

class MoveRightCommand : public Command
{
public:
	MoveRightCommand(PlayerPrefab* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveRight(); return true; }
private:
	PlayerPrefab* m_pPlayer;
};

class MoveUpCommand : public Command
{
public:
	MoveUpCommand(PlayerPrefab* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveUp(); return true; }
private:
	PlayerPrefab* m_pPlayer;
};

class MoveDownCommand : public Command
{
public:
	MoveDownCommand(PlayerPrefab* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveDown(); return true; }
private:
	PlayerPrefab* m_pPlayer;
};

class PlayMusic : public Command
{
public:
	PlayMusic(std::string& name)
		:m_Name{name}
	{
	}
	bool Execute() override {
		ServiceLocator<SoundManager, SoundManager>::GetService().EditSound(m_Name, SoundManager::Action::Play);
		return true;
	}

private:
	std::string m_Name;
};

class StopMusic : public Command
{
public:
	StopMusic(std::string& name)
		:m_Name{ name }
	{
	}
	bool Execute() override {
		ServiceLocator<SoundManager, SoundManager>::GetService().EditSound(m_Name, SoundManager::Action::Stop);
		return true;
	}

private:
	std::string m_Name;
};

class PlayEffect : public Command
{
public:
	PlayEffect(std::string& name, int loops)
		:m_Name{ name }
		,m_Loops(loops)
	{
	}
	bool Execute() override { ServiceLocator<SoundManager, SoundManager>::GetService().EditSound(
		m_Name, SoundManager::Action::Play, m_Loops, false); return true;
	}

private:
	std::string m_Name;
	int m_Loops;
};