#pragma once
#include "BaseComponent.h"
#include "ObserverEvents.h"
#include <list>
class ObserverComponent;
class SubjectComponent :
	public BaseComponent
{
public:
	SubjectComponent(GameObject* pOwner, const std::string& tag = "");
	virtual ~SubjectComponent();

	SubjectComponent(const SubjectComponent& other) = delete;
	SubjectComponent(SubjectComponent&& other) = delete;
	SubjectComponent& operator=(const SubjectComponent& other) = delete;
	SubjectComponent& operator=(SubjectComponent&& other) = delete;

	void Notify(const GameObject* go, Event event);
	void AddObserver(ObserverComponent* pObserver);
	void RemoveObserver(ObserverComponent* pObserver);

	void Update() override {}
	void Render() const override {}
	void OnLoad() override {}

protected:
	std::list<ObserverComponent*> m_Observers;
};

