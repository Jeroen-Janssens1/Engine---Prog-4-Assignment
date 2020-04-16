#pragma once
#include "Command.h"
#include "SceneManager.h"

class JumpCommand : public Command
{
private:
	static int index;
	void Jump()
	{
		std::cout << "You jumped!" << std::endl;
		//ServiceLocator::GetAudio().Playsound(0);
		dae::SceneManager::GetInstance().SetActiveScene(index);
		index = (index + 1) % dae::SceneManager::GetInstance().GetAmountOfScenes();
	}

public:
	bool Execute() override { Jump(); return true; }
};
int JumpCommand::index = 0;

class FireCommand : public Command
{
private:
	void Fire()
	{
		std::cout << "You fired!" << std::endl;
		//ServiceLocator::GetAudio().Playsound(1);
	}

public:
	bool Execute() override { Fire(); return true; }
};

class DuckCommand : public Command
{
private:
	void Duck()
	{
		std::cout << "You ducked!" << std::endl;
		//ServiceLocator::GetAudio().Playsound(2);
	}

public:
	bool Execute() override { Duck(); return true; }
};

class FartCommand : public Command
{
private:
	void Fart()
	{
		std::cout << "You farted!" << std::endl;
		//ServiceLocator::GetAudio().Playsound(3);
	}

public:
	bool Execute() override { Fart(); return true; }
};