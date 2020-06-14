#pragma once
#include "Command.h"
#include "SceneManager.h"
#include "SoundManager.h"
#include "PlayerBehaviour.h"
#include "MainMenu.h"
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
		:m_pMenu{menu}
	{
	}
	bool Execute() override { m_pMenu->SelectUp(); return true; }

private:
	MainMenu* m_pMenu;

};

class MenuSelectDownCommand : public Command
{
public:
	MenuSelectDownCommand(MainMenu* menu)
		:m_pMenu{ menu }
	{
	}
	bool Execute() override { m_pMenu->SelectDown(); return true; }

private:
	MainMenu* m_pMenu;

};

class MenuConfirmSelectionCommand : public Command
{
public:
	MenuConfirmSelectionCommand(MainMenu* menu)
		:m_pMenu{ menu }
	{
	}
	bool Execute() override { return m_pMenu->ConfirmSelection(); }

private:
	MainMenu* m_pMenu;

};