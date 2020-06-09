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

PlayerBehaviour::PlayerBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	, m_GameTime{ GameTimeService }
{

}

void PlayerBehaviour::Initialize(InputManager* pInput, b2World* pPhysicsWorld, unsigned int controllerIndx)
{
	m_pInput = pInput;
	m_Transform = new TransformComponent(m_pOwner);
	m_pOwner->AddComponent(m_Transform);

	// this object is both the observer and the notifier. Doesn't normally happen but this is purely for testing purposes
	auto* achievementObserver = new Achievement(m_pOwner);
	m_pOwner->AddComponent(achievementObserver);
	auto* inputSubject = new InputSubject(m_pOwner);
	inputSubject->AddObserver(achievementObserver);
	m_pOwner->AddComponent(inputSubject);

	auto renderComponent = new RenderComponent(m_pOwner, m_Transform, 64, 64, true, 32, 32, 16, 0);
	m_pOwner->AddComponent(renderComponent);

	renderComponent->SetTexture("Resources/AvatarSheet.png");


	// after setting up the render component, create all the animations you need
	int nrOfAnimations = 6;
	std::vector<Animation*> animations{};
	b2Vec2 framePos{};
	framePos.x = 0;
	framePos.y = 0;
	for (int i{}; i < nrOfAnimations; i++)
	{
		int nrOfFrames{};
		std::vector<b2Vec2> framePositions{};
		if (i == 0)
			nrOfFrames = 13;
		else if (i == 1)
			nrOfFrames = 8;
		else if (i == 2)
			nrOfFrames = 7;
		else
			nrOfFrames = 10;
		framePos.x = 0;
		// for this you first need to set up the frame positions
		for (int j{}; j < nrOfFrames; j++)
		{
			framePositions.push_back(framePos);
			framePos.x += 32;
		}
		// Then you push back the new animation
		if (i == 0)
		{
			animations.push_back(new Animation("Idle", renderComponent, framePositions, 0.12f));
			framePos.y += 32;
		}
		else if (i == 1)
		{
			animations.push_back(new Animation("Walking", renderComponent, framePositions, 0.12f));
			framePos.y += (32 * 6);
		}
		else if (i == 2)
		{
			animations.push_back(new Animation("Death", renderComponent, framePositions, 0.12f, false));
			framePos.y -= (32 * 5);
		}
		else if (i == 3)
		{
			animations.push_back(new Animation("Attack1", renderComponent, framePositions, 0.12f, false, true));
			framePos.y += 32;
		}
		else if (i == 4)
		{
			animations.push_back(new Animation("Attack2", renderComponent, framePositions, 0.12f, false, true));
			framePos.y += 32;
		}
		else if (i == 5)
			animations.push_back(new Animation("Attack3", renderComponent, framePositions, 0.12f, false, true));
	}
	// now you need to set up the transitions
	Transition* transition = new Transition(animations[1], true, false, "Walking");
	animations[0]->AddTransition(transition);
	transition = new Transition(animations[0], true, false, "Still");
	animations[1]->AddTransition(transition);
	transition = new Transition(animations[2], true, false, "Death");
	animations[0]->AddTransition(transition);
	animations[1]->AddTransition(transition);
	animations[3]->AddTransition(transition);
	animations[4]->AddTransition(transition);
	animations[5]->AddTransition(transition);

	transition = new Transition(animations[3], true, false, "Attack1");
	animations[0]->AddTransition(transition);
	animations[1]->AddTransition(transition);
	animations[5]->AddTransition(transition);

	transition = new Transition(animations[4], true, false, "Attack2");
	animations[3]->AddTransition(transition);

	transition = new Transition(animations[5], true, false, "Attack3");
	animations[4]->AddTransition(transition);

	transition = new Transition(animations[0]);
	animations[3]->AddTransition(transition);
	animations[4]->AddTransition(transition);
	animations[5]->AddTransition(transition);

	//Make the actual animator component, this component will take care of all the deletion of the animations and transitions
	m_pAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pAnimator);

	// Controller Input
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadRight, new MoveRightCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadLeft, new MoveLeftCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadUp, new MoveUpCommand(this));

	// Keyboard Input
	if (controllerIndx == 0)
	{
		m_pInput->MapCommand('D', new MoveRightCommand(this));
		m_pInput->MapCommand('A', new MoveLeftCommand(this));
		m_pInput->MapCommand('W', new MoveUpCommand(this));
		m_Transform->SetPosition(0, 0, 0);
	}
	else
	{
		m_pInput->MapCommand(VK_RIGHT, new MoveRightCommand(this));
		m_pInput->MapCommand(VK_LEFT, new MoveLeftCommand(this));
		m_pInput->MapCommand(VK_UP, new MoveUpCommand(this));
		m_Transform->SetPosition(100, 50, 0);
	}

	m_pBox2D = new Box2DComponent(m_pOwner, m_Transform, pPhysicsWorld, renderComponent->GetWidth(), renderComponent->GetHeight(), 1.f, 0.1f, true);
	m_pOwner->AddComponent(m_pBox2D);

}

void PlayerBehaviour::Update()
{
	//GameObject::Update();

	if (m_pInput->IsPressed('0'))
	{
		m_pAnimator->SetTrigger("Death");
		m_pBox2D->SetIsEnabled(false);
		m_Vel.x = 0;
	}

	if (m_pInput->IsPressed('9'))
	{
		m_pAnimator->ResetAnimator();
		m_pBox2D->SetIsEnabled(true);
	}

	if (m_pAnimator->GetCurrentStateName() == "Death")
		return;

	if (m_pInput->IsPressed('8'))
	{
		m_Vel.x = 0;
		std::string trigger{ "" };
		if (m_pAnimator->GetCurrentStateName() == "Attack1")
			trigger = "Attack2";
		else if (m_pAnimator->GetCurrentStateName() == "Attack2")
			trigger = "Attack3";
		else
			trigger = "Attack1";
		m_pAnimator->SetTrigger(trigger);
	}
	else if (m_pAnimator->GetCurrentStateName() == "Attack1" || m_pAnimator->GetCurrentStateName() == "Attack2" || m_pAnimator->GetCurrentStateName() == "Attack3")
	{
		m_pAnimator->ResetTrigger("Attack1");
		m_pAnimator->ResetTrigger("Attack2");
		m_pAnimator->ResetTrigger("Attack3");
	}

	if (m_pAnimator->GetCurrentStateName() == "Attack1" || m_pAnimator->GetCurrentStateName() == "Attack2"
		|| m_pAnimator->GetCurrentStateName() == "Attack3")
		return;
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

	// apply gravity
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	m_pBox2D->SetVelocity(m_Vel.x, curVel.y);
	m_Vel.x = 0;

}

void PlayerBehaviour::MoveUp()
{
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	if (curVel.y > 0)
		m_JumpForce = b2Vec2(0.f, -200.f);
}

void PlayerBehaviour::MoveDown()
{
}

void PlayerBehaviour::MoveRight()
{
	m_Vel.x += m_Speed * m_GameTime.GetElapsed();
}

void PlayerBehaviour::MoveLeft()
{
	m_Vel.x += -m_Speed * m_GameTime.GetElapsed();
}
