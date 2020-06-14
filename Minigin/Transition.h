#pragma once
#include <string>
class State;
class Transition
{
public:
	Transition(State* pNewState, bool usesCondition = false, bool conditionIsBool = false, const std::string& conditionName = "");
	~Transition()=default;

	Transition(const Transition& other) = delete;
	Transition(Transition&& other) = delete;
	Transition& operator=(const Transition& other) = delete;
	Transition& operator=(Transition&& other) = delete;

	bool GetConditionName(std::string& Name) const;
	bool Condition();
	void SetTrigger() { m_Trigger = true; };
	void SetBool(bool value) { m_Bool = value; };
	void ResetTrigger() { m_Trigger = false; }
	State* GetNewState() const { return m_pNewState; }
	bool ConditionIsBool() const { return m_HasBool; }

private:
	std::string m_ConditionName;
	bool m_HasTrigger;
	bool m_HasBool;
	bool m_Bool;
	bool m_Trigger;
	State* m_pNewState;
	
	
};

