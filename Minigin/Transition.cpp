#include "MiniginPCH.h"
#include "Transition.h"
#include "State.h"

Transition::Transition(State* pNewState, bool usesCondition, bool conditionIsBool, const std::string& conditionName)
	:m_pNewState{pNewState}
	,m_HasTrigger(usesCondition && !conditionIsBool)
	,m_ConditionName{conditionName}
	,m_Trigger{false}
	,m_Bool{false}
	,m_HasBool{usesCondition && conditionIsBool}
{
}

bool Transition::GetConditionName(std::string& conditionName) const
{
	if (m_HasTrigger || m_HasBool)
	{
		conditionName = m_ConditionName;
		return true;
	}
	return false;
}

bool Transition::Condition()
{
	if (m_HasTrigger)
	{
		if (m_Trigger)
		{
			ResetTrigger();
			return true;
		}
		return false;
	}
	if (m_HasBool)
		return m_Bool;
	// always return true if there is no condition to transition
	return true;
}
