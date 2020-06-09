#include "Achievement.h"
#include <iostream>

Achievement::Achievement(GameObject* pOwner)
	:ObserverComponent(pOwner)
{
}

void Achievement::onNotify(const GameObject* go, Event event)
{
	ObserverComponent::onNotify(go, event);

	switch (event)
	{
	case Event::PressedA:
		UnlockAchievement();
	}
}

void Achievement::UnlockAchievement()
{
	if (m_AchievementUnlocked)
		return;
	m_AchievementUnlocked = true;
	std::cout << "You got Achievement: 'Press A!'" << std::endl;
}
