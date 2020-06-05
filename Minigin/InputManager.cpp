#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Command.h"

InputManager::~InputManager()
{
	delete[] m_XButtons;
	for (size_t i{}; i < m_Commands.size(); i++)
	{
		delete m_Commands[i];
		m_Commands[i] = nullptr;
	}
	m_Commands.clear();
}

InputManager::InputManager()
{
	// This array is used so that you can map your physical buttons to different XInput buttons
	// in case your controller uses different mappings
	m_XButtons = new int[int(ControllerButton::StructSize)];
	m_Commands.resize(int(ControllerButton::StructSize));
	m_CommandUsesPressed.resize(int(ControllerButton::StructSize));
	m_CommandWasPressed.resize(int(ControllerButton::StructSize));
	m_XButtons[int(ControllerButton::ButtonA)] = XINPUT_GAMEPAD_A;
	m_Commands[int(ControllerButton::ButtonA)] = new NullCommand();

	m_XButtons[int(ControllerButton::ButtonB)] = XINPUT_GAMEPAD_B;
	m_Commands[int(ControllerButton::ButtonB)] = new NullCommand();

	m_XButtons[int(ControllerButton::ButtonX)] = XINPUT_GAMEPAD_X;
	m_Commands[int(ControllerButton::ButtonX)] = new NullCommand();

	m_XButtons[int(ControllerButton::ButtonY)] = XINPUT_GAMEPAD_Y;
	m_Commands[int(ControllerButton::ButtonY)] = new NullCommand();

	m_XButtons[int(ControllerButton::DPadUp)] = XINPUT_GAMEPAD_DPAD_UP;
	m_Commands[int(ControllerButton::DPadUp)] = new NullCommand();

	m_XButtons[int(ControllerButton::DPadDown)] = XINPUT_GAMEPAD_DPAD_DOWN;
	m_Commands[int(ControllerButton::DPadDown)] = new NullCommand();

	m_XButtons[int(ControllerButton::DPadLeft)] = XINPUT_GAMEPAD_DPAD_LEFT;
	m_Commands[int(ControllerButton::DPadLeft)] = new NullCommand();

	m_XButtons[int(ControllerButton::DPadRight)] = XINPUT_GAMEPAD_DPAD_RIGHT;
	m_Commands[int(ControllerButton::DPadRight)] = new NullCommand();

	// Get first active controller on device and recover its state.
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	m_ControllerId = -1;
	for (DWORD i = 0; i < XUSER_MAX_COUNT && m_ControllerId == -1; i++)
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &m_CurrentState) == ERROR_SUCCESS)
			m_ControllerId = i;
	}
	// all commands are set to checking for held down mode instead of checking for singular press mode
	for (size_t i{}; i < m_CommandUsesPressed.size(); i++)
	{
		m_CommandUsesPressed[i] = false;
		m_CommandWasPressed[i] = false;
	}
}

void InputManager::MapCommand(ControllerButton button, Command* command, bool usesPressedCheck)
{
	delete m_Commands[int(button)];
	m_Commands[int(button)] = command;
	m_CommandUsesPressed[int(button)] = usesPressedCheck;
}

bool InputManager::ProcessInput()
{
	bool keepPlaying = true;
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);
	// keyboard input
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			keepPlaying = false;
			return keepPlaying;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	// controller input
	// Execute function based on button pressed
	for (size_t i{}; i < m_Commands.size(); i++)
	{
		if (IsPressed(ControllerButton(i)))
		{
			keepPlaying = m_Commands[i]->Execute();
		}
		if (!keepPlaying)
			return keepPlaying;
	}

	

	return keepPlaying;
}

bool InputManager::IsPressed(ControllerButton button)
{
	// First check if the button is in the Pressed checking mode.
	// If it is, check if the button was already pressed last frame,
	// if it was we didn't press it this frame so we return false.
	bool isPressed = m_CurrentState.Gamepad.wButtons & m_XButtons[int(button)];

	if (m_CommandUsesPressed[int(button)])
	{
		if (m_CommandWasPressed[int(button)])
		{
			m_CommandWasPressed[int(button)] = isPressed;
			return false;
		}

		
	}
	m_CommandWasPressed[int(button)] = isPressed;
	// If the button isn't in Pressed checking mode we just check if the button
	// is held down or not
	return isPressed;
}

// for multiplayer
/*
Store the number of players somewhere.
When playing single player, this is 1
When player coop or versus, this can be any number (this specific game would set it to 2)
Check if the playerIndx in IsPressed is within the amount of players.
If it is, check if it is within the amount of controller states, if it is this player is using
a controler, otherwise he is using the keyboard.
Check for controller/keyboard input accordingly.
*/