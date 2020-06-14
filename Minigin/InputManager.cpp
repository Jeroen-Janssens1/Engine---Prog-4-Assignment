#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Command.h"

InputManager::~InputManager()
{
	for (size_t i{}; i < XUSER_MAX_COUNT; i++)
	{
		delete[] m_XButtons[i];
		for (size_t j{}; j < m_Commands[i].size(); j++)
		{
			delete m_Commands[i][j];
			m_Commands[i][j] = nullptr;
		}
		m_Commands[i].clear();
	}
	m_Commands.clear();
	m_XButtons.clear();
	for (size_t i{}; i < m_KbCommands.size(); i++)
	{
		delete m_KbCommands[i];
		m_KbCommands[i] = nullptr;
	}
}

InputManager::InputManager()
{
	// Set up the keyboard input
	// take into account all VK code possibilities
	m_KbCommands.resize(255, nullptr);
	for (size_t i{}; i < m_KbCommands.size(); i++)
		m_KbCommands[i] = new NullCommand();
	m_KbCommandUsesPressed.resize(255, false);
	m_KbCommandWasPressed.resize(255, false);
	// Set up the controller input
	// This array is used so that you can map your physical buttons to different XInput buttons
	// in case your controller uses different mappings
	// Get all connected controllers
	m_CurrentState.resize(XUSER_MAX_COUNT, XINPUT_STATE());
	m_IsControllerActive.resize(XUSER_MAX_COUNT, false);
	for (size_t i{}; i < m_CurrentState.size(); i++)
		ZeroMemory(&m_CurrentState[i], sizeof(XINPUT_STATE));
	m_XInputToControllerIndx.resize(4, -1);
	m_CurControllerIndx = 0;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE tempState;
		ZeroMemory(&tempState, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &tempState) == ERROR_SUCCESS)
		{
			// if controller index not take yet
			if (m_XInputToControllerIndx[i] == -1)
			{
				m_XInputToControllerIndx[i] = m_CurControllerIndx;
				m_IsControllerActive[m_CurControllerIndx] = true;
				ZeroMemory(&m_CurrentState[m_CurControllerIndx], sizeof(XINPUT_STATE));
				m_CurrentState[m_CurControllerIndx] = tempState;
				m_CurControllerIndx++;
			}
			else
			{
				m_IsControllerActive[m_XInputToControllerIndx[i]] = true;
				ZeroMemory(&m_CurrentState[m_XInputToControllerIndx[i]], sizeof(XINPUT_STATE));
				m_CurrentState[m_XInputToControllerIndx[i]] = tempState;
			}
		}
	}
	

	for (size_t i{}; i < m_CurrentState.size(); i++)
	{
		m_XButtons.push_back(new int[int(ControllerButton::StructSize)]);

		std::vector<Command*> commands;
		commands.resize(int(ControllerButton::StructSize));
		m_Commands.push_back(commands);

		std::vector<bool> usesPressed;
		usesPressed.resize(int(ControllerButton::StructSize));
		m_CommandUsesPressed.push_back(usesPressed);
		m_CommandWasPressed.push_back(usesPressed);

		// push back all controller controls that are currently supported and their hardware mappings
		m_XButtons.back()[int(ControllerButton::ButtonA)] = XINPUT_GAMEPAD_A;
		m_Commands.back()[int(ControllerButton::ButtonA)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::ButtonB)] = XINPUT_GAMEPAD_B;
		m_Commands.back()[int(ControllerButton::ButtonB)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::ButtonX)] = XINPUT_GAMEPAD_X;
		m_Commands.back()[int(ControllerButton::ButtonX)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::ButtonY)] = XINPUT_GAMEPAD_Y;
		m_Commands.back()[int(ControllerButton::ButtonY)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::DPadUp)] = XINPUT_GAMEPAD_DPAD_UP;
		m_Commands.back()[int(ControllerButton::DPadUp)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::DPadDown)] = XINPUT_GAMEPAD_DPAD_DOWN;
		m_Commands.back()[int(ControllerButton::DPadDown)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::DPadLeft)] = XINPUT_GAMEPAD_DPAD_LEFT;
		m_Commands.back()[int(ControllerButton::DPadLeft)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::DPadRight)] = XINPUT_GAMEPAD_DPAD_RIGHT;
		m_Commands.back()[int(ControllerButton::DPadRight)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::Start)] = XINPUT_GAMEPAD_START;
		m_Commands.back()[int(ControllerButton::Start)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::ShoulderL)] = XINPUT_GAMEPAD_LEFT_SHOULDER;
		m_Commands.back()[int(ControllerButton::ShoulderL)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::ShoulderR)] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
		m_Commands.back()[int(ControllerButton::ShoulderR)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::StickL)] = XINPUT_GAMEPAD_LEFT_THUMB;
		m_Commands.back()[int(ControllerButton::StickL)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::StickR)] = XINPUT_GAMEPAD_RIGHT_THUMB;
		m_Commands.back()[int(ControllerButton::StickR)] = new NullCommand();

		m_XButtons.back()[int(ControllerButton::Back)] = XINPUT_GAMEPAD_BACK;
		m_Commands.back()[int(ControllerButton::Back)] = new NullCommand();
	}

	// all commands are set to checking for held down mode instead of checking for singular press mode
	for (size_t i{}; i < m_CommandUsesPressed.size(); i++)
	{
		for (size_t j{}; j < m_CommandUsesPressed[i].size(); j++)
		{
			m_CommandUsesPressed[i][j] = false;
			m_CommandWasPressed[i][j] = false;
		}
	}
}

void InputManager::MapCommand(unsigned int controllerIndx, ControllerButton button, Command* command, bool usesPressedCheck)
{
	if (!(m_IsControllerActive.size() > controllerIndx))
		return;
	for (size_t i{}; i < m_Commands.size(); i++)
	{
		auto it = std::find(m_Commands[i].cbegin(), m_Commands[i].cend(), command);
		// can't map an already existing command to the same button
		if (it != m_Commands[i].cend())
			return;
	}
	auto it = std::find(m_KbCommands.cbegin(), m_KbCommands.cend(), command); // command may not exist in KbCommands either to prevent deletion errors
	if (it != m_KbCommands.cend())
		return;
	// actual mapping code
	delete m_Commands[controllerIndx][int(button)];
	m_Commands[controllerIndx][int(button)] = command;
	m_CommandUsesPressed[controllerIndx][int(button)] = usesPressedCheck;
}


void InputManager::MapCommand(int keyboardCode, Command* command, bool usesPressedCheck)
{
	for (size_t i{}; i < m_Commands.size(); i++)
	{
		auto it = std::find(m_Commands[i].cbegin(), m_Commands[i].cend(), command);
		// can't map an already existing command to the same button
		if (it != m_Commands[i].cend())
			return;
	}
	auto it = std::find(m_KbCommands.cbegin(), m_KbCommands.cend(), command);
	if (it != m_KbCommands.cend())
		return;
	// actual mapping code
	delete m_KbCommands[keyboardCode];
	m_KbCommands[keyboardCode] = command;
	m_KbCommandUsesPressed[keyboardCode] = usesPressedCheck;
}

bool InputManager::ProcessInput()
{
	bool keepPlaying = true;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++)
	{
		XINPUT_STATE tempState;
		ZeroMemory(&tempState, sizeof(XINPUT_STATE));
		if (XInputGetState(i, &tempState) == ERROR_SUCCESS)
		{
			if (m_XInputToControllerIndx[i] == -1)
			{
				m_XInputToControllerIndx[i] = m_CurControllerIndx;
				m_IsControllerActive[m_CurControllerIndx] = true;
				ZeroMemory(&m_CurrentState[m_CurControllerIndx], sizeof(XINPUT_STATE));
				m_CurrentState[m_CurControllerIndx] = tempState;
				m_CurControllerIndx++;
			}
			else
			{
				m_IsControllerActive[m_XInputToControllerIndx[i]] = true;
				ZeroMemory(&m_CurrentState[m_XInputToControllerIndx[i]], sizeof(XINPUT_STATE));
				m_CurrentState[m_XInputToControllerIndx[i]] = tempState;
			}
		}
	}

	// SDL Based Input
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
		// ignore inactive controllers
		if (!m_IsControllerActive[i])
			continue;

		for (size_t j{}; j < m_Commands[i].size(); j++)
		{
			if (IsPressed(unsigned int(i), ControllerButton(j)))
			{
				keepPlaying = m_Commands[i][j]->Execute();
			}
			if (!keepPlaying)
				return keepPlaying;
		}
	}
	// keyboard input
	for (size_t i{}; i < m_KbCommands.size(); i++)
	{
		if (IsPressed(int(i)))
		{
			keepPlaying = m_KbCommands[i]->Execute();
		}
		if (!keepPlaying)
			return keepPlaying;
	}
	return keepPlaying;
}

bool InputManager::IsPressed(unsigned int controllerIndx, ControllerButton button)
{
	if (!(m_IsControllerActive.size() > controllerIndx))
		return false;
	if (!m_IsControllerActive[controllerIndx])
		return false;
	// First check if the button is in the Pressed checking mode.
	// If it is, check if the button was already pressed last frame,
	// if it was we didn't press it this frame so we return false.
	bool isPressed = m_CurrentState[controllerIndx].Gamepad.wButtons & m_XButtons[controllerIndx][int(button)];
	if (m_CommandUsesPressed[controllerIndx][int(button)])
	{
		if (m_CommandWasPressed[controllerIndx][int(button)])
		{
			m_CommandWasPressed[controllerIndx][int(button)] = isPressed;
			return false;
		}
	}
	m_CommandWasPressed[controllerIndx][int(button)] = isPressed;
	// If the button isn't in Pressed checking mode we just check if the button
	// is held down or not
	return isPressed;
}

bool InputManager::IsPressed(int keyboardCode)
{
	// check keyboard input using GetKeyState
	bool isPressed = (GetKeyState(keyboardCode) & 0x8000);
	if (m_KbCommandUsesPressed[keyboardCode])
	{
		if (m_KbCommandWasPressed[keyboardCode])
		{
			m_KbCommandWasPressed[keyboardCode] = isPressed;
			return false;
		}
	}
	m_KbCommandWasPressed[keyboardCode] = isPressed;
	// If the button isn't in Pressed checking mode we just check if the button
	// is held down or not
	return isPressed;
}