#pragma once
#include "Command.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "PlayerBehaviour.h"
#include "MainMenu.h"
 ////////////////////////////////////
// Only for debugging purposes
class NextSceneCommand : public Command
{
private:
	static int index;
	void NextScene()
	{
		//ServiceLocator::GetAudio().Playsound(0);
		ServiceLocator<SceneManager, SceneManager>::GetService().SetActiveScene(index);
		index = (index + 1) % ServiceLocator<SceneManager, SceneManager>::GetService().GetAmountOfScenes();
	}

public:
	bool Execute() override { NextScene(); return true; }
};
 // Debugging commands end here
//////////////////////////////////////

class AttackCommand : public Command
{
public:
	AttackCommand(PlayerBehaviour* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->Attack(); return true; }

private:
	PlayerBehaviour* m_pPlayer;
};

class MoveLeftCommand : public Command
{
public:
	MoveLeftCommand(PlayerBehaviour* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveLeft(); return true; }
private:
	PlayerBehaviour* m_pPlayer;
};

class MoveRightCommand : public Command
{
public:
	MoveRightCommand(PlayerBehaviour* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveRight(); return true; }
private:
	PlayerBehaviour* m_pPlayer;
};

class MoveUpCommand : public Command
{
public:
	MoveUpCommand(PlayerBehaviour* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveUp(); return true; }
private:
	PlayerBehaviour* m_pPlayer;
};

class MoveDownCommand : public Command
{
public:
	MoveDownCommand(PlayerBehaviour* pPlayer)
	{
		m_pPlayer = pPlayer;
	}
	bool Execute() override { m_pPlayer->MoveDown(); return true; }
private:
	PlayerBehaviour* m_pPlayer;
};

class MenuSelectUpCommand : public Command
{
public:
	MenuSelectUpCommand(MainMenu* menu)
		:m_Menu{menu}
	{
	}
	bool Execute() override { m_Menu->SelectUp(); return true; }

private:
	MainMenu* m_Menu;

};

class MenuSelectDownCommand : public Command
{
public:
	MenuSelectDownCommand(MainMenu* menu)
		:m_Menu{ menu }
	{
	}
	bool Execute() override { m_Menu->SelectDown(); return true; }

private:
	MainMenu* m_Menu;

};

class MenuConfirmSelectionCommand : public Command
{
public:
	MenuConfirmSelectionCommand(MainMenu* menu)
		:m_Menu{ menu }
	{
	}
	bool Execute() override { return m_Menu->ConfirmSelection(); }

private:
	MainMenu* m_Menu;

};