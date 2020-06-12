#include "PlayerBehaviour.h"
#include "TransformComponent.h"
#include "ServiceLocator.h"
#include "InputManager.h"
#include "InputCommands.h"
#include "GameTime.h"
#include "RenderComponent.h"
#include "SceneManager.h"
#include "Box2DComponent.h"
#include "glm\common.hpp"
#include "SceneManager.h"
#include "Scene.h"
#include "AnimatorIncludes.h"
#include "InputSubject.h"
#include "Achievement.h"
#include "Services.h"
#include "ContactListener.h"

#include <iostream>

PlayerBehaviour::PlayerBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	, m_GameTime{ GameTimeService }
	,m_FootSensorCounter{0}
{

}

void PlayerBehaviour::Initialize(InputManager* pInput, b2World* pPhysicsWorld, unsigned int controllerIndx, float xPos, float yPos)
{
	m_IsGrounded = false;
	m_pInput = pInput;
	m_pTransform = new TransformComponent(m_pOwner);
	m_pOwner->AddComponent(m_pTransform);

	m_pRenderComp = new RenderComponent(m_pOwner, m_pTransform, "", 32, 32, true, 18, 16, 0, 0);
	m_pOwner->AddComponent(m_pRenderComp);

	if(controllerIndx == 0)
		m_pRenderComp->SetTexture("Resources/Player1.png");


	// after setting up the render component, create all the animations you need
	int nrOfAnimations = 2;
	std::vector<Animation*> animations{};
	b2Vec2 framePos{};
	framePos.x = 0;
	framePos.y = 0;
	for (int i{}; i < nrOfAnimations; i++)
	{
		int nrOfFrames{};
		std::vector<b2Vec2> framePositions{};
		if (i == 0)
			nrOfFrames = 2;
		else if (i == 1)
			nrOfFrames = 4;
		framePos.x = 0;
		// for this you first need to set up the frame positions
		for (int j{}; j < nrOfFrames; j++)
		{
			framePositions.push_back(framePos);
			framePos.x += 18;
		}
		// Then you push back the new animation
		if (i == 0)
		{
			animations.push_back(new Animation("Idle", m_pRenderComp, framePositions, 0.3f));
		}
		else if (i == 1)
			animations.push_back(new Animation("Walking", m_pRenderComp, framePositions, 0.15f));
	}
	// now you need to set up the transitions
	Transition* transition = new Transition(animations[1], true, false, "Walking");
	animations[0]->AddTransition(transition);
	transition = new Transition(animations[0], true, false, "Still");
	animations[1]->AddTransition(transition);

	//Make the actual animator component, this component will take care of all the deletion of the animations and transitions
	m_pAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pAnimator);

	m_pTransform->SetPosition(xPos, yPos, 0);

	// Controller Input
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadRight, new MoveRightCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadLeft, new MoveLeftCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadUp, new MoveUpCommand(this), true);

	// Keyboard Input
	if (controllerIndx == 0)
	{
		m_pInput->MapCommand('D', new MoveRightCommand(this));
		m_pInput->MapCommand('A', new MoveLeftCommand(this));
		m_pInput->MapCommand('W', new MoveUpCommand(this), true);
	}
	else
	{
		m_pInput->MapCommand(VK_RIGHT, new MoveRightCommand(this));
		m_pInput->MapCommand(VK_LEFT, new MoveLeftCommand(this));
		m_pInput->MapCommand(VK_UP, new MoveUpCommand(this), true);
	}

	m_pBox2D = new Box2DComponent(m_pOwner, m_pTransform, pPhysicsWorld, 
		m_pRenderComp->GetWidth(), m_pRenderComp->GetHeight(), "FootSensor", 1.f, 1.f, true, b2Vec2(0.f, 0.f), false, true);
	m_pOwner->AddComponent(m_pBox2D);

	// add footsensor fixture to this body
	m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);

	m_pBox2D->SetCollisionCallbackScript(this);

}

void PlayerBehaviour::Update()
{
	m_IsGrounded = false;
	if (m_Vel.x > m_Speed)
	{
		m_Vel.x = m_Speed;
	}

	std::string trigger = "Still";
	if (abs(m_Vel.x) > 0.5)
	{
		trigger = "Walking";
		m_pAnimator->SetTrigger(trigger);
	}
	else
		m_pAnimator->SetTrigger(trigger);
	m_pBox2D->ApplyForce(m_JumpForce);
	m_JumpForce = b2Vec2_zero;

	// apply the x velocity
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	m_pBox2D->SetVelocity(m_Vel.x, curVel.y);
	m_Vel.x = 0;

}

void PlayerBehaviour::MoveUp()
{
	if (m_FootSensorCounter != 0)
		m_JumpForce = b2Vec2(0.f, -500.f);
}

void PlayerBehaviour::MoveDown()
{
}

void PlayerBehaviour::MoveRight()
{
	m_Vel.x += m_Speed * m_GameTime.GetElapsed();
	m_pRenderComp->SetIsFlipped(false);
}

void PlayerBehaviour::MoveLeft()
{
	m_Vel.x += -m_Speed * m_GameTime.GetElapsed();
	m_pRenderComp->SetIsFlipped(true);
}

// Collision Callback Functions //
void PlayerBehaviour::OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = contact->GetFixtureA();
	else
		fixture = contact->GetFixtureB();
	if (fixture->IsSensor() && (other->GetGameObject()->GetTag() == "TileMap" || other->GetGameObject()->GetTag() == "LevelEdge"))
		IncrementFootCounter();
}

void PlayerBehaviour::OnContactEnd(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = contact->GetFixtureA();
	else
		fixture = contact->GetFixtureB();
	if (fixture->IsSensor() && (other->GetGameObject()->GetTag() == "TileMap" || other->GetGameObject()->GetTag() == "LevelEdge"))
		DecrementFootCounter();
}