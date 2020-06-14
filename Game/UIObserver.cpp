#include "UIObserver.h"
#include <iostream>
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "PlayerBehaviour.h"

UIObserver::UIObserver(GameObject* pOwner, TextRenderComponent* livesText, TextRenderComponent* scoreText)
	:ObserverComponent(pOwner)
	,m_pLives{livesText}
	,m_pScore{scoreText}
{
}

void UIObserver::onNotify(const GameObject* go, Event event)
{
	ObserverComponent::onNotify(go, event);
	if (go->GetTag() != "Player" && go->GetTag() != "Enemy")
		return;
	switch (event)
	{
	case Event::LivesUpdate:
		m_pLives->SetText(std::to_string(go->GetComponent<PlayerBehaviour>()->GetLives()));
		break;
	case Event::ScoreUpdate:
		m_pScore->SetText(std::to_string(go->GetComponent<PlayerBehaviour>()->GetScore()));
		break;
	}
}
