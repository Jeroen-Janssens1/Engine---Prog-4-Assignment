#include "MiniginPCH.h"
#include "State.h"
#include "Transition.h"


State::State(const std::string& name)
	:m_Name{name}
{
}

void State::Exit()
{
	m_pBoolTransition = nullptr;
}

bool State::HandleTransitions(State** newState)
{
	// if we got here because of a bool condition we don't check for transitions untill the bool has been reset
	if (m_pBoolTransition)
	{
		if (m_pBoolTransition->Condition())
			return false;
	}
	for (auto* transition : m_pTransitions)
	{
		if (transition->Condition())
		{
			*newState = transition->GetNewState();
			if (transition->ConditionIsBool())
			{
				(*newState)->m_pBoolTransition = transition;
			}
			return true;
		}
	}
	return false;
}
