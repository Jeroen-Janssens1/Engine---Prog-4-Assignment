#pragma once
#include "BaseComponent.h"
#include "ObserverEvents.h"
#include <list>

class SubjectComponent;
class ObserverComponent :
	public BaseComponent
{
public:
	ObserverComponent(GameObject* pOwner, const std::string& tag = "");
	virtual ~ObserverComponent();

	ObserverComponent(const ObserverComponent& other) = delete;
	ObserverComponent(ObserverComponent&& other) = delete;
	ObserverComponent& operator=(const ObserverComponent& other) = delete;
	ObserverComponent& operator=(ObserverComponent&& other) = delete;

	void AddSubject(SubjectComponent* pSubject) { m_Subjects.push_front(pSubject); };
	void OnLoad() override {}

	virtual void onNotify(const GameObject* go, Event event);

protected:
	std::list<SubjectComponent*> m_Subjects;
};

