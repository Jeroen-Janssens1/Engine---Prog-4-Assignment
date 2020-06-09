#pragma once
#include "ObserverComponent.h"
class Achievement final :
	public ObserverComponent
{
public:
	Achievement(GameObject* pOwner);

	void onNotify(const GameObject* go, Event event) override;
	virtual void Update() override {};
	virtual void Render() const override {};

private:
	void UnlockAchievement();
	bool m_AchievementUnlocked = false;

};

