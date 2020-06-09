#include "InputSubject.h"
#include "ServiceLocator.h"
#include "InputManager.h"

InputSubject::InputSubject(GameObject* pOwner)
	:SubjectComponent(pOwner)
{
}

void InputSubject::Update()
{
	auto& input = ServiceLocator<InputManager, InputManager>::GetService();

	if (input.IsPressed(0, ControllerButton::ButtonA))
	{
		Notify(m_pOwner, Event::PressedA);
	}
}
