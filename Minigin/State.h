#pragma once
#include <vector>
class Transition;
class State
{
public:
	State(const std::string& name);
	virtual ~State()=default;

	virtual void Update()=0;
	virtual void Entry()=0;
	virtual void Exit();

	virtual bool HandleTransitions(State** newState);

	const std::vector<Transition*>& GetTransitions() const { return m_pTransitions; }

	const std::string& GetName() const { return m_Name; }

	// push_back if the transition isn't already present in the state
	void AddTransition(Transition* pTransition) { if(std::find(m_pTransitions.cbegin(), m_pTransitions.cend(), pTransition) == m_pTransitions.cend())
		m_pTransitions.push_back(pTransition); };

private:
	std::vector<Transition*> m_pTransitions;
	Transition* m_pBoolTransition;
	std::string m_Name;

};

