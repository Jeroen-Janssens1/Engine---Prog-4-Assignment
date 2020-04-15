#pragma once
#include <XInput.h>
#include "Singleton.h"
class Command;
enum class ControllerButton
{
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	StructSize // THIS MUST ALWAYS BE THE LAST ENTRY IN THE STRUCT!
};

class InputManager final
{
public:
	InputManager();
	~InputManager();
	bool ProcessInput();
	bool IsPressed(ControllerButton button) const;
	template<typename T>
	void MapCommand(ControllerButton button)
	{
		m_Commands[int(button)].reset();
		m_Commands[int(button)] = std::make_unique<T>();

		/*if (button == ControllerButton::ButtonA)
		{
			m_ButtonA.reset();
			m_ButtonA = std::make_unique<T>();
			return;
		}
		if (button == ControllerButton::ButtonB)
		{
			m_ButtonB.reset();
			m_ButtonB = std::make_unique<T>();
			return;
		}
		if (button == ControllerButton::ButtonX)
		{
			m_ButtonX.reset();
			m_ButtonX = std::make_unique<T>();
			return;
		}
		if (button == ControllerButton::ButtonY)
		{
			m_ButtonY.reset();
			m_ButtonY = std::make_unique<T>();
			return;
		}*/
	}
private:
	int m_ControllerId;
	int* m_XButtons;

	XINPUT_STATE m_CurrentState;

	// all commands for our buttons will be stored in here
	std::vector < std::unique_ptr<Command>> m_Commands;

	//std::unique_ptr<Command> m_ButtonX;
	//std::unique_ptr<Command> m_ButtonY;
	//std::unique_ptr<Command> m_ButtonA;
	//std::unique_ptr<Command> m_ButtonB;
};