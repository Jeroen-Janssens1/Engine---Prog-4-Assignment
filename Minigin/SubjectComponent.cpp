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

void SubjectComponent::AddObserver(ObserverComponent* pObserver)
{
	if (std::find(m_Observers.cbegin(), m_Observers.cend(), pObserver) == m_Observers.cend())
	{
		m_Observers.push_back(pObserver);
		pObserver->AddSubject(this);
	}
}

void SubjectComponent::RemoveObserver(ObserverComponent* pObserver)
{
	m_Observers.remove(pObserver);
}
