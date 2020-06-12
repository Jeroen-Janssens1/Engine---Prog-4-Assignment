#include "MiniginPCH.h"
#include "ObserverComponent.h"
#include "GameObject.h"
#include "SubjectComponent.h"

ObserverComponent::ObserverComponent(GameObject* pOwner, const std::string& tag)
	:BaseComponent(pOwner, tag)
{
}

ObserverComponent::~ObserverComponent()
{
	for (auto* subject : m_Subjects)
	{
		subject->RemoveObserver(this);
	}
}

void ObserverComponent::onNotify(const GameObject* go, Event event)
{
	switch (event)
	{
	case Event::SubjectDestroyed:
		auto* goSubject = go->GetComponent<SubjectComponent>();
		if (!goSubject)
			return;
		for (auto* subject : m_Subjects)
		{
			if (subject == goSubject)
			{
				subject->RemoveObserver(this);
				m_Subjects.remove(subject);
			}
		}
	}
}
