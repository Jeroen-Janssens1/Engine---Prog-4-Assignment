#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>
#include "Command.h"

InputManager::~InputManager()
{
	delete[] m_XButtons;
}

InputManager::InputManager()
{
	// This array is used so that you can map your physical buttons to different XInput buttons
	// in case your controller uses different mappings
	m_XButtons = new int[int(ControllerButton::StructSize)];
	m_Commands.resize(int(ControllerButton::StructSize));
	m_XButtons[int(ControllerButton::ButtonA)] = XINPUT_GAMEPAD_A;
	m_Commands[int(ControllerButton::ButtonA)] = std::make_unique<NullCommand>();

	m_XButtons[int(ControllerButton::ButtonB)] = XINPUT_GAMEPAD_B;
	m_Commands[int(ControllerButton::ButtonB)] = std::make_unique<NullCommand>();

	m_XButtons[int(ControllerButton::ButtonX)] = XINPUT_GAMEPAD_X;
	m_Commands[int(ControllerButton::ButtonX)] = std::make_unique<NullCommand>();

	m_XButtons[int(ControllerButton::ButtonY)] = XINPUT_GAMEPAD_Y;
	m_Commands[int(ControllerButton::ButtonY)] = std::make_unique<NullCommand>();

	// Get first active controller on device and recover its state.
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	m_ControllerId = -1;
	for (DWORD i = 0; i < XUSER_MAX_COUNT && m_ControllerId == -1; i++)
	{
		ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

		if (XInputGetState(i, &m_CurrentState) == ERROR_SUCCESS)
			m_ControllerId = i;
	}

	// Map the base commands to each button
	//m_ButtonA = std::make_unique<FireCommand>();
	//m_ButtonB = std::make_unique<JumpCommand>();
	//m_ButtonX = std::make_unique<DuckCommand>();
	//m_ButtonY = std::make_unique<FartCommand>();
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
	XInputGetState(m_ControllerId, &m_CurrentState);

	// Execute function based on button pressed
	for (int i{}; i < m_Commands.size(); i++)
	{
		if (IsPressed(ControllerButton(i)))
			keepPlaying = m_Commands[i]->Execute();
		if (!keepPlaying)
			return keepPlaying;
	}
	/*if (IsPressed(ControllerButton::ButtonA)) m_ButtonA->Execute();
	if (IsPressed(ControllerButton::ButtonB)) m_ButtonB->Execute();
	if (IsPressed(ControllerButton::ButtonX)) m_ButtonX->Execute();
	if (IsPressed(ControllerButton::ButtonY)) m_ButtonY->Execute();*/

	

	return keepPlaying;
}

bool InputManager::IsPressed(ControllerButton button) const
{
	return m_CurrentState.Gamepad.wButtons & m_XButtons[int(button)];
}

