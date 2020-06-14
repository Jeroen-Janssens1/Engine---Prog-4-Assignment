#include "MiniginPCH.h"
#include "SpriteAnimatorComponent.h"
#include "Animation.h"
#include "Transition.h"

SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* pOwner, std::vector<Animation*>& animations, const std::string& tag)
	:BaseComponent(pOwner, tag)
	,m_Animations{animations}
{
	m_pActiveAnimation = m_Animations[0];
	m_pActiveAnimation->Entry();
	// Add all transitions (no duplicates) to m_Transitions for deletion during cleanup
	std::vector<Transition*> transitions;
	for (size_t i{}; i < m_Animations.size(); i++)
	{
		transitions = m_Animations[i]->GetTransitions();
		for (size_t j{}; j < transitions.size(); j++)
		{
			if (std::find(m_Transitions.cbegin(), m_Transitions.cend(), transitions[j]) == m_Transitions.cend())
			{
				m_Transitions.push_back(transitions[j]);
			}
		}
	}
}

SpriteAnimatorComponent::~SpriteAnimatorComponent()
{
	for (size_t i{}; i < m_Animations.size(); i++)
		delete m_Animations[i];
	for (size_t i{}; i < m_Transitions.size(); i++)
		delete m_Transitions[i];
	m_Animations.clear();
	m_Transitions.clear();
}

void SpriteAnimatorComponent::Update()
{
	m_pActiveAnimation->Update();
	// check transitions for current state
	State* newState = nullptr;
	if (m_pActiveAnimation->HandleTransitions(&newState))
	{
		// transition from one to the other
		m_pActiveAnimation->Exit();
		m_pActiveAnimation = newState;
		m_pActiveAnimation->Entry();
	}
}

void SpriteAnimatorComponent::ResetAnimator()
{
	m_pActiveAnimation->Exit();
	m_pActiveAnimation = m_Animations[0];
	m_pActiveAnimation->Entry();
}

void SpriteAnimatorComponent::SetTrigger(const std::string& name)
{
	std::string triggerName;
	for (size_t i{}; i < m_Transitions.size(); i++)
	{
		if (m_Transitions[i]->GetConditionName(triggerName))
		{
			if (triggerName == name)
			{
				m_Transitions[i]->SetTrigger();
				return;
			}
		}
	}
}

void SpriteAnimatorComponent::ResetTrigger(const std::string& name)
{
	std::string triggerName;
	for (size_t i{}; i < m_Transitions.size(); i++)
	{
		if (m_Transitions[i]->GetConditionName(triggerName))
		{
			if (triggerName == name)
			{
				m_Transitions[i]->ResetTrigger();
				return;
			}
		}
	}
}

void SpriteAnimatorComponent::SetBool(const std::string& name, bool value)
{
	std::string boolName;
	for (size_t i{}; i < m_Transitions.size(); i++)
	{
		if (m_Transitions[i]->GetConditionName(boolName))
		{
			if (boolName == name)
			{
				m_Transitions[i]->SetBool(value);
				return;
			}
		}
	}
}

const std::string& SpriteAnimatorComponent::GetCurrentStateName() const
{
	 return m_pActiveAnimation->GetName();
}
