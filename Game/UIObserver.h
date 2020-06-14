#pragma once
#include "ObserverComponent.h"
class TextRenderComponent;
class UIObserver final :
	public ObserverComponent
{
public:
	UIObserver(GameObject* pOwner, TextRenderComponent* livesText, TextRenderComponent* scoreText );

	void onNotify(const GameObject* go, Event event) override;
	virtual void Update() override {};
	virtual void Render() const override {};

private:
	TextRenderComponent* m_pLives;
	TextRenderComponent* m_pScore;

};

