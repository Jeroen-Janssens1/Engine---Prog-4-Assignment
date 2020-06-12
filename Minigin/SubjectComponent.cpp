#include "MiniginPCH.h"
#include "SubjectComponent.h"
#include "ObserverComponent.h"

SubjectComponent::SubjectComponent(GameObject* pOwner, const std::string& tag)
	:BaseComponent(pOwner, tag)
{
}

SubjectComponent::~SubjectComponent()
{
	for (auto* observer : m_Observers)
		observer->onNotify(m_pOwner, Event::SubjectDestroyed);
}

void SubjectComponent::Notify(const GameObject* go, Event event)
{
	for (auto* observer : m_Observers)
		observer->onNotify(go, event);
}

void SubjectComponent::AddObserver(ObserverComponent* observer)
{
	if (std::find(m_Observers.cbegin(), m_Observers.cend(), observer) == m_Observers.cend())
	{
		m_Observers.push_back(observer);
		observer->AddSubject(this);
	}
}

void SubjectComponent::RemoveObserver(ObserverComponent* observer)
{
	m_Observers.remove(observer);
}
