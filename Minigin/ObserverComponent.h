#pragma once
#include "BaseComponent.h"
#include "ObserverEvents.h"
#include <list>

class SubjectComponent;
class ObserverComponent :
	public BaseComponent
{
public:
	ObserverComponent(GameObject* pOwner);
	virtual ~ObserverComponent();

	void AddSubject(SubjectComponent* subject) { m_Subjects.push_front(subject); };

	ObserverComponent(const ObserverComponent& other) = delete;
	ObserverComponent(ObserverComponent&& other) = delete;
	ObserverComponent& operator=(const ObserverComponent& other) = delete;
	ObserverComponent& operator=(ObserverComponent&& other) = delete;

	virtual void onNotify(const GameObject* go, Event event);

protected:
	std::list<SubjectComponent*> m_Subjects;

private:


};

