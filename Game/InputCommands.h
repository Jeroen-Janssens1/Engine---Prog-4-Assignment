#pragma once
#include "Command.h"
#include "SceneManager.h"
 ////////////////////////////////////
// Only for debugging purposes
class NextSceneCommand : public Command
{
private:
	static int index;
	void NextScene()
	{
		std::cout << "You jumped!" << std::endl;
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
	bool Execute() override { Function(); return true; }
};

class MoveRightCommand : public Command
{
public:
	bool Execute() override { Function(); return true; }
};

class MoveUpCommand : public Command
{
public:
	bool Execute() override { Function(); return true; }
};

class MoveDownCommand : public Command
{
public:
	bool Execute() override { Function(); return true; }
};