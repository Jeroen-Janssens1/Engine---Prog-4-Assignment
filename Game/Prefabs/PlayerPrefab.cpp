#include "PlayerPrefab.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "InputManager.h"
#include "InputCommands.h"
#include "GameTime.h"
#include "RenderComponent.h"

PlayerPrefab::PlayerPrefab()
	:m_Input{ServiceLocator<InputManager, InputManager>::GetService()}
{

}

void PlayerPrefab::Initialize(std::shared_ptr<GameObject> thisSmart)
{
	m_Transform = std::make_shared<TransformComponent>(TransformComponent(thisSmart));
	std::shared_ptr<BaseComponent> addComponent;
	addComponent = std::static_pointer_cast<BaseComponent>(m_Transform);
	AddComponent(addComponent);

	auto renderComponent = std::make_shared<RenderComponent>(RenderComponent(thisSmart, m_Transform, 20, 20, true, 9, 9, 16, 16, 16, 0));
	addComponent = renderComponent;
	AddComponent(addComponent);

	m_Transform->SetPosition(50, 50, 0);
	renderComponent->SetTexture("Resources/levelSprites.png");

	InputManager& input = ServiceLocator<InputManager, InputManager>::GetService();

	input.MapCommand<MoveRightCommand>(ControllerButton::DPadRight);
	input.MapCommand<MoveLeftCommand>(ControllerButton::DPadLeft);
	input.MapCommand<MoveUpCommand>(ControllerButton::DPadUp);
	input.MapCommand<MoveDownCommand>(ControllerButton::DPadDown);
}

void PlayerPrefab::Update()
{

}

void PlayerPrefab::MoveUp()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y - (30* GameTime::GetInstance().GetElapsed()),
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveDown()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y + (30 * GameTime::GetInstance().GetElapsed()),
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveRight()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x + (30 * GameTime::GetInstance().GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);
}

void PlayerPrefab::MoveLeft()
{
	m_Transform->SetPosition(m_Transform->GetPosition().x - (30 * GameTime::GetInstance().GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);
}
