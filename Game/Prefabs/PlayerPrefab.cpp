#include "PlayerPrefab.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "InputManager.h"
#include "InputCommands.h"
#include "GameTime.h"
#include "RenderComponent.h"

PlayerPrefab::PlayerPrefab()
	:m_Input{ServiceLocator<InputManager, InputManager>::GetService()}
	,m_GameTime{ServiceLocator<GameTime, GameTime>::GetService()}
{

}

void PlayerPrefab::Initialize()
{
	m_Transform = new TransformComponent(this);
	AddComponent(m_Transform);

	auto renderComponent = new RenderComponent(this, m_Transform, 20, 20, true, 9, 9, 16, 16, 16, 0);
	AddComponent(renderComponent);

	m_Transform->SetPosition(50, 50, 0);
	renderComponent->SetTexture("Resources/levelSprites.png");


	m_Input.MapCommand(ControllerButton::DPadRight, new MoveRightCommand(this));
	m_Input.MapCommand(ControllerButton::DPadLeft, new MoveLeftCommand(this));
	m_Input.MapCommand(ControllerButton::DPadUp, new MoveUpCommand(this));
	m_Input.MapCommand(ControllerButton::DPadDown, new MoveDownCommand(this));
}

void PlayerPrefab::Update()
{

}

void PlayerPrefab::MoveUp()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y - (30* m_GameTime.GetElapsed()),
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveDown()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y + (30 * m_GameTime.GetElapsed()),
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveRight()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x + (30 * m_GameTime.GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveLeft()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x - (30 * m_GameTime.GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);
}
