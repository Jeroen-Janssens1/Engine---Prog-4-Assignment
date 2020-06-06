#include "PlayerPrefab.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "InputManager.h"
#include "InputCommands.h"
#include "GameTime.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Box2DComponent.h"
#include "glm\common.hpp"

PlayerPrefab::PlayerPrefab()
	:GameObject()
	,m_Input{ServiceLocator<InputManager, InputManager>::GetService()}
	,m_GameTime{ServiceLocator<GameTime, GameTime>::GetService()}
{

}

void PlayerPrefab::Initialize(b2World* pPhysicsWorld)
{
	m_Transform = new TransformComponent(this);
	AddComponent(m_Transform);

	auto renderComponent = new RenderComponent(this, m_Transform, 20, 20, true, 9, 9, 16, 16, 16, 0);
	AddComponent(renderComponent);

	m_Transform->SetPosition(50, 50, 0);
	renderComponent->SetTexture("Resources/levelSprites.png");

	m_pBox2D = new Box2DComponent(this, m_Transform, pPhysicsWorld, renderComponent->GetWidth(), renderComponent->GetHeight(), 0.0f, 1.f, true);
	AddComponent(m_pBox2D);

	// Controller Input
	m_Input.MapCommand(ControllerButton::DPadRight, new MoveRightCommand(this));
	m_Input.MapCommand(ControllerButton::DPadLeft, new MoveLeftCommand(this));
	m_Input.MapCommand(ControllerButton::DPadUp, new MoveUpCommand(this));

	// Keyboard Input
	m_Input.MapCommand('D', new MoveRightCommand(this));
	m_Input.MapCommand('A', new MoveLeftCommand(this));
	m_Input.MapCommand(' ', new MoveUpCommand(this));
}

void PlayerPrefab::Update()
{
	GameObject::Update();
	if (m_Vel.x > m_Speed)
	{
		m_Vel.x = m_Speed;
	}

	// apply gravity
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	m_pBox2D->SetVelocity(m_Vel.x, curVel.y);
	m_Vel.x = 0;

	

	b2Vec2 vec{};
	
}

void PlayerPrefab::MoveUp()
{
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	if(curVel.y > 0)
		m_pBox2D->ApplyForce(b2Vec2(0.f, -200));
	//m_Vel.y += -m_Speed * m_GameTime.GetElapsed();
	/*m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y - (30* m_GameTime.GetElapsed()),
		m_Transform->GetPosition().z);*/
}

void PlayerPrefab::MoveDown()
{
	//m_Vel.y += m_Speed * m_GameTime.GetElapsed();

	/*m_Transform->SetPosition(m_Transform->GetPosition().x, m_Transform->GetPosition().y + (30 * m_GameTime.GetElapsed()),
		m_Transform->GetPosition().z);*/
}

void PlayerPrefab::MoveRight()
{
	m_Vel.x += m_Speed * m_GameTime.GetElapsed();

	/*m_Transform->SetPosition(m_Transform->GetPosition().x + (30 * m_GameTime.GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);*/
}

void PlayerPrefab::MoveLeft()
{
	m_Vel.x += -m_Speed * m_GameTime.GetElapsed();

	/*m_Transform->SetPosition(m_Transform->GetPosition().x - (30 * m_GameTime.GetElapsed()), m_Transform->GetPosition().y,
		m_Transform->GetPosition().z);*/
}
