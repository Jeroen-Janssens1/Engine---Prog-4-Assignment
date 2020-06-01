#pragma once
#include <Windows.h>
#include <XInput.h>
#include "Singleton.h"
#include <vector>
#include <memory>
class Command;
enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	DPadUp,
	DPadDown,
	DPadLeft,
	DPadRight,
	StructSize // THIS MUST ALWAYS BE THE LAST ENTRY IN THE STRUCT!
};

class InputManager final
{
public:
	InputManager();
	~InputManager();
	bool ProcessInput();
	bool IsPressed(ControllerButton button);
	void MapCommand(ControllerButton button, Command* command, bool usesPressedCheck = false)
	{
		delete m_Commands[int(button)];
		m_Commands[int(button)] = command;
		m_CommandUsesPressed[int(button)] = usesPressedCheck;
	}
private:
	int m_ControllerId;
	int* m_XButtons;

	XINPUT_STATE m_CurrentState;
	std::vector<bool> m_CommandWasPressed;

	// all commands for our buttons will be stored in here
	std::vector <Command*> m_Commands;
	std::vector<bool> m_CommandUsesPressed;
};