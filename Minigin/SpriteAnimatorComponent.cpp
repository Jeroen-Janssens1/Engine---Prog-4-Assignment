#include "MiniginPCH.h"
#include "SpriteAnimatorComponent.h"
#include "Animation.h"
#include "Transition.h"

SpriteAnimatorComponent::SpriteAnimatorComponent(GameObject* pOwner, std::vector<Animation*>& animations, const std::string& tag)
	:BaseComponent(pOwner, tag)
	,m_Animations{animations}
{
	m_ActiveAnimation = m_Animations[0];
	m_ActiveAnimation->Entry();
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
	m_ActiveAnimation->Update();
	// check transitions for current state
	State* newState = nullptr;
	if (m_ActiveAnimation->HandleTransitions(&newState))
	{
		// transition from one to the other
		m_ActiveAnimation->Exit();
		m_ActiveAnimation = newState;
		m_ActiveAnimation->Entry();
	}
}

void SpriteAnimatorComponent::Render() const
{
}

void SpriteAnimatorComponent::ResetAnimator()
{
	m_ActiveAnimation->Exit();
	m_ActiveAnimation = m_Animations[0];
	m_ActiveAnimation->Entry();
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
	 return m_ActiveAnimation->GetName();
}
