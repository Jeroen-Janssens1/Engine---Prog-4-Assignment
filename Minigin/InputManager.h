#pragma once
#include <Windows.h>
#include <XInput.h>
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
	Start,
	Back,
	ShoulderL,
	ShoulderR,
	StickL,
	StickR,
	StructSize // THIS MUST ALWAYS BE THE LAST ENTRY IN THE STRUCT!
};

class InputManager final
{
public:
	InputManager();
	~InputManager();
	bool ProcessInput();
	bool IsPressed(ControllerButton button);
	bool IsPressed(int keyboardCode);
	void MapCommand(ControllerButton button, Command* command, bool usesPressedCheck = false);
	void MapCommand(int keyboardCode, Command* command, bool usesPressedCheck = false);
private:

	// Controller related variables //

	int m_ControllerId;
	int* m_XButtons;

	XINPUT_STATE m_CurrentState;
	std::vector<bool> m_CommandWasPressed;

	// all commands for our buttons will be stored in here
	std::vector <Command*> m_Commands;
	std::vector<bool> m_CommandUsesPressed;



	// Keyboard related variables //

	// commands for keyboard binds
	std::vector<Command*> m_KbCommands;
	std::vector<bool> m_KbCommandUsesPressed;
	std::vector<bool> m_KbCommandWasPressed;
};