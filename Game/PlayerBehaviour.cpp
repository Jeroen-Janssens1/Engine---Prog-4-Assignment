#include "PlayerBehaviour.h"
#include "InputCommands.h"
#include "Scene.h"
#include "AnimatorIncludes.h"
#include "InputManager.h"
#include "GameObject.h"
#include "SubjectComponent.h"
#include "Services.h"
#include "Bubble.h"
#include "Components.h"
#include "ZenBehaviour.h"
#include "Boulder.h"
#include <iostream>

PlayerBehaviour::PlayerBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	, m_GameTime{ GameTimeService }
	,m_FootSensorCounter{0}
{

}

void PlayerBehaviour::Initialize(InputManager* pInput, b2World* pPhysicsWorld, unsigned int controllerIndx, float xPos, float yPos,
	unsigned int lives, unsigned int score, bool isMaita)
{
	m_IsMaita = isMaita;
	m_Lives = 4;
	m_Score = 0;
	m_AttackTimer = m_AttackCooldown;
	m_Lives = lives;
	m_Score = score;
	m_SpawnPos.x = xPos;
	m_SpawnPos.y = yPos;
	m_IsHit = false;
	m_NrOfOverlappers = 0;
	m_IgnoreCollisions = false;
	m_pInput = pInput;
	m_pTransform = new TransformComponent(m_pOwner);
	m_pOwner->AddComponent(m_pTransform);

	m_pRenderComp = new RenderComponent(m_pOwner, m_pTransform, "", 32, 32, true, 18, 16, 0, 0);
	m_pOwner->AddComponent(m_pRenderComp);

	// set correct texture
	if (m_IsMaita)
		m_pRenderComp->SetTexture("Resources/enemySheet.png");
	else if (controllerIndx == 0)
		m_pRenderComp->SetTexture("Resources/Player1.png");
	else
		m_pRenderComp->SetTexture("Resources/Player2.png");


	// after setting up the render component, create all the animations you need
	// Each animation takes a vector of b2Vec2. These are used as positions on the sprite sheet, each position representing 1 frame of the animation
	int nrOfAnimations = 3;
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
		else if (i == 2)
		{
			nrOfFrames = 8;
			framePos.y = 54;
		}
		framePos.x = 0;
		// set the frame positions up
		for (int j{}; j < nrOfFrames; j++)
		{
			framePositions.push_back(framePos);
			if (j == 3 && i == 2)
				framePos.x = 0;
			else
				framePos.x += 18;
		}
		// Then you push back the new animation
		if (i == 0)
		{
			animations.push_back(new Animation("Idle", m_pRenderComp, framePositions, 0.3f));
		}
		else if (i == 1)
			animations.push_back(new Animation("Walking", m_pRenderComp, framePositions, 0.15f));
		else if (i == 2)
			animations.push_back(new Animation("Death", m_pRenderComp, framePositions, 0.15f, false, true));
	}
	// now you need to set up the transitions
	Transition* transition = new Transition(animations[1], true, false, "Walking");
	animations[0]->AddTransition(transition);
	transition = new Transition(animations[0], true, false, "Still");
	animations[1]->AddTransition(transition);
	transition = new Transition(animations[2], true, false, "Death");
	animations[0]->AddTransition(transition);
	animations[1]->AddTransition(transition);
	transition = new Transition(animations[0]);
	animations[2]->AddTransition(transition);

	//Make the actual animator component
	m_pAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pAnimator);

	// set up for the second animator
	nrOfAnimations = 4;
	animations.clear();
	framePos.x = 0;
	framePos.y = 21;
	for (int i{}; i < nrOfAnimations; i++)
	{
		int nrOfFrames{};
		std::vector<b2Vec2> framePositions{};
		framePos.x = 0;
		if (i == 3)
			nrOfFrames = 2;
		else if (i == 1)
		{
			nrOfFrames = 2;
			framePos.x = 124;
		}
		else if (i == 2)
		{
			nrOfFrames = 8;
			framePos.x = 252;
		}
		else if (i == 0)
			nrOfFrames = 1;
		for (int j{}; j < nrOfFrames; j++)
		{
			framePositions.push_back(framePos);
			if (i == 2 && j == 3)
				framePos.x = 252;
			else
				framePos.x += 21;
		}
		if (i == 0)
			animations.push_back(new Animation("Idle", m_pRenderComp, framePositions, 0.3f));
		else if (i == 1)
			animations.push_back(new Animation("Bubbled", m_pRenderComp, framePositions, 0.3f));
		else if (i == 2)
			animations.push_back(new Animation("Death", m_pRenderComp, framePositions, 0.3f, false, true));
		else if (i == 3)
			animations.push_back(new Animation("Walking", m_pRenderComp, framePositions, 0.25f));
	}
	transition = new Transition(animations[3], true, false, "Walking");
	animations[0]->AddTransition(transition);
	transition = new Transition(animations[0], true, false, "Still");
	animations[3]->AddTransition(transition);
	animations[1]->AddTransition(transition);
	transition = new Transition(animations[2], true, false, "Death");
	animations[1]->AddTransition(transition);
	transition = new Transition(animations[0]);
	animations[2]->AddTransition(transition);
	transition = new Transition(animations[1], true, false, "Bubbled");
	animations[3]->AddTransition(transition);
	animations[0]->AddTransition(transition);
	m_pMaitaAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pMaitaAnimator);

	// select the correct animator
	if (m_IsMaita)
		m_pAnimator->SetIsEnabled(false);
	else
		m_pMaitaAnimator->SetIsEnabled(false);

	m_pTransform->SetPosition(xPos, yPos, 0);

	// Controller Input
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadRight, new MoveRightCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadLeft, new MoveLeftCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadUp, new MoveUpCommand(this), true);
	m_pInput->MapCommand(controllerIndx, ControllerButton::DPadDown, new MoveDownCommand(this));
	m_pInput->MapCommand(controllerIndx, ControllerButton::ButtonB, new AttackCommand(this), true);
	// Keyboard Input
	if (controllerIndx == 0)
	{
		m_pInput->MapCommand('D', new MoveRightCommand(this));
		m_pInput->MapCommand('A', new MoveLeftCommand(this));
		m_pInput->MapCommand('W', new MoveUpCommand(this), true);
		m_pInput->MapCommand('S', new MoveDownCommand(this));
		m_pInput->MapCommand(' ', new AttackCommand(this), true);
	}
	else
	{
		m_pInput->MapCommand(VK_RIGHT, new MoveRightCommand(this));
		m_pInput->MapCommand(VK_LEFT, new MoveLeftCommand(this));
		m_pInput->MapCommand(VK_UP, new MoveUpCommand(this), true);
		m_pInput->MapCommand(VK_DOWN, new MoveDownCommand(this));
		m_pInput->MapCommand(VK_RSHIFT, new AttackCommand(this));
	}
	m_pBox2D = new Box2DComponent(m_pOwner, m_pTransform, pPhysicsWorld, 
		m_pRenderComp->GetWidth() - 10.f, m_pRenderComp->GetHeight(), "FootSensor", 1.f, 2.f, true, b2Vec2(0.f, 0.f), false, true);
	m_pOwner->AddComponent(m_pBox2D);
	// add footsensor fixture to this body
	m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_pBox2D->SetCollisionCallbackScript(this);
	m_pSubject = new SubjectComponent(m_pOwner);
	m_pOwner->AddComponent(m_pSubject);
}

void PlayerBehaviour::Update()
{
	if (m_AttackTimer < m_AttackCooldown)
	{
		m_AttackTimer += m_GameTime.GetElapsed();
	}

	if (m_pMaitaAnimator->GetCurrentStateName() == "Bubbled" && m_IsMaita) // Versus Game Mode
	{
		m_pMaitaAnimator->ResetTrigger("Still");
		m_pBox2D->SetVelocity(0.f, 0.f);
		m_BubbledTimer += m_GameTime.GetElapsed();
		if (m_BubbledTimer >= m_BubbledDuration)
		{
			m_pMaitaAnimator->SetTrigger("Still");
			m_IsBubbled = false;
		}
		return;
	}

	if (m_pAnimator->GetCurrentStateName() == "Death" || (m_pMaitaAnimator->GetCurrentStateName() == "Death" && m_IsMaita))
	{
		if (m_IsHit)
		{
			m_IsHit = false;
			// disable physics emulation
			m_pBox2D->SetIsEnabled(false);
			if (m_Lives == 0)
			{
				Kill();
				return;
			}
		}
		return;
	}

	if (!m_pBox2D->GetIsEnabled())
	{
		// re-enable physics and respawn the player
		m_pBox2D->SetIsEnabled(true);
		m_pBox2D->SetPosition(m_SpawnPos.x, m_SpawnPos.y);
	}

	std::string trigger = "Still";
	if (abs(m_Vel.x) > 0.5)
	{
		trigger = "Walking";
		if(!m_IsMaita)
			m_pAnimator->SetTrigger(trigger);
		else
			m_pMaitaAnimator->SetTrigger(trigger);
	}
	else
	{
		if (!m_IsMaita)
			m_pAnimator->SetTrigger(trigger);
		else
			m_pMaitaAnimator->SetTrigger(trigger);
	}
	m_pBox2D->ApplyForce(m_JumpForce);
	m_JumpForce = b2Vec2_zero;

	// apply the x velocity
	b2Vec2 curVel;
	m_pBox2D->GetVelocity(curVel);
	m_pBox2D->SetVelocity(m_Vel.x, curVel.y);
	m_Vel.x = 0;
	if (curVel.y > 0.2f)
		m_IgnoreCollisions = false;
	if (m_NrOfOverlappers == 0)
		m_IsDropping = false;
}

void PlayerBehaviour::OnLoad()
{
	m_pBox2D->SetPosition(m_SpawnPos.x, m_SpawnPos.y);
}

void PlayerBehaviour::MoveUp()
{
	b2Vec2 vel{};
	m_pBox2D->GetVelocity(vel);
	// only jump if grounded and not already going up (negative y means up)
	if (m_FootSensorCounter != 0 && vel.y >= -0.1)
	{
		SoundService.EditSound(1, SoundManager::Action::Play, 0, false);
		m_JumpForce = b2Vec2(0.f, -600.f);
		m_IgnoreCollisions = true;
	}
}

void PlayerBehaviour::MoveDown()
{
	m_IsDropping = true;
	m_IgnoreCollisions = true;
}

void PlayerBehaviour::MoveRight()
{
	m_Vel.x += m_Speed;
	if(!m_IsMaita)
		m_pRenderComp->SetIsFlipped(false);
	else
		m_pRenderComp->SetIsFlipped(true);
}

void PlayerBehaviour::MoveLeft()
{
	m_Vel.x += -m_Speed;
	if(!m_IsMaita)
		m_pRenderComp->SetIsFlipped(true);
	else
		m_pRenderComp->SetIsFlipped(false);
}

void PlayerBehaviour::Attack()
{
	if (!m_pOwner->GetIsEnabled())
		return;
	if ((m_pAnimator->GetCurrentStateName() == "Death" && !m_IsMaita) || (m_pMaitaAnimator->GetCurrentStateName() == "Death" && m_IsMaita)
		|| (m_pMaitaAnimator->GetCurrentStateName() == "Bubbled" && m_IsMaita))
		return;
	if (m_AttackTimer >= m_AttackCooldown)
	{
		SoundService.EditSound(2, SoundManager::Action::Play, 0, false);
		if(!m_IsMaita)
			SpawnBubble();
		else
			ThrowBoulder();
		m_AttackTimer = 0;
	}
}

void PlayerBehaviour::SpawnBubble()
{
	float spawnVel = 1;
	// spawn a bubble in the current facing direction
	if (m_pRenderComp->GetIsFlipped()) // if flipped, left direction
	{
		spawnVel = -1;
	}
	auto* go = new GameObject("Bubble");
	auto* tc = new TransformComponent(go);
	go->AddComponent(tc);
	auto* rc = new RenderComponent(go, tc, "", 8, 8, true, 8, 8, 19, 40);
	rc->SetTexture("Resources/Player1.png");
	go->AddComponent(rc);
	auto* collider = new Box2DComponent(go, tc, SceneService.GetActiveScene()->GetPhysicsWorld(), rc->GetWidth(), rc->GetHeight(), "", 0.f, 1.f, true,
		b2Vec2{ 0.f, 0.f }, true, true, false, false);
	go->AddComponent(collider);
	Bubble* bubble = new Bubble(go, spawnVel * 300, collider);
	go->AddComponent(bubble);
	collider->SetPosition(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y);
	collider->SetCollisionCallbackScript(bubble);
	SceneService.GetActiveScene()->Add(go);
}
void PlayerBehaviour::ThrowBoulder()
{
	if (m_AttackTimer < m_AttackCooldown)
		return;

	m_AttackTimer = 0;
	float spawnVel = 1;
	// spawn a bubble in the current facing direction
	if (!m_pRenderComp->GetIsFlipped()) // if not flipped, left direction
	{
		spawnVel = -1;
	}
	auto* go = new GameObject("Enemy");
	auto* tc = new TransformComponent(go);
	go->AddComponent(tc);
	auto* rc = new RenderComponent(go, tc, "", 30, 30, true, 16, 16, 5, 0);
	rc->SetTexture("Resources/Boulder.png");
	go->AddComponent(rc);
	auto* collider = new Box2DComponent(go, tc, SceneService.GetActiveScene()->GetPhysicsWorld(), rc->GetWidth(), rc->GetHeight(), "", 0.f, 1.f, true,
		b2Vec2{ 0.f, 0.f }, true, true, false, false);
	go->AddComponent(collider);
	Boulder* boulder = new Boulder(go, spawnVel * 100, collider);
	go->AddComponent(boulder);
	collider->SetPosition(m_pTransform->GetPosition().x, m_pTransform->GetPosition().y);
	collider->SetCollisionCallbackScript(boulder);

	SceneService.GetActiveScene()->Add(go);
}

// Collision Callback Functions // => The way these calls work is you first check ShouldCollide, if that returns false everything else is skipped
// if it returns true, OnContactBegin is called, then PreSolve and PostSolve get called repeatedly during the collision to resolve it.
// Once the two fixtures/collision boxes get seperated again the OnContactEnd functions gets called

// OnContactBegin and End are used for checking if we are grounded and should be allowed to jump or not
void PlayerBehaviour::OnContactBegin(b2Contact* pContact, Box2DComponent* pthisCollider, Box2DComponent* pOther)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(pContact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(pContact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	b2Fixture* otherFixture = nullptr;
	if (collider1 == pthisCollider)
	{
		fixture = pContact->GetFixtureA();
		otherFixture = pContact->GetFixtureB();
	}
	else
	{
		fixture = pContact->GetFixtureB();
		otherFixture = pContact->GetFixtureA();
	}
	if (fixture->IsSensor() && (pOther->GetGameObject()->GetTag() == "TileMap" || pOther->GetGameObject()->GetTag() == "LevelEdge"))
		IncrementFootCounter();

	if (!fixture->IsSensor() && pOther->GetGameObject()->GetTag() == "TileMap")
		m_NrOfOverlappers++;

	// handle collisions with enemies appropriately
	if (pOther->GetGameObject()->GetTag() == "Enemy" && !fixture->IsSensor() && !m_IsMaita)
	{
		ZenBehaviour* enemy = pOther->GetGameObject()->GetComponent<ZenBehaviour>("Enemy");
		PlayerBehaviour* playerEnemy = pOther->GetGameObject()->GetComponent<PlayerBehaviour>();
		if (enemy)
		{
			if (!otherFixture->IsSensor())
			{
				if (enemy->GetIsBubbled())
				{
					enemy->Kill();
					m_Score += enemy->GetScore();
					m_pSubject->Notify(m_pOwner, Event::ScoreUpdate);
				}
				else if (!m_IsHit)
				{
					// make sure player gets taken out of the physics emulation, play death animation, remove 1 life
					m_IsHit = true;
					m_pAnimator->SetTrigger("Death");
					if (m_Lives > 0)
					{
						m_Lives--;
						m_pSubject->Notify(m_pOwner, Event::LivesUpdate);
					}
				}
			}
		}
		else if (pOther->GetGameObject()->GetComponent<Boulder>() && !m_IsHit)
		{
			// make sure player gets taken out of the physics emulation, play death animation, remove 1 life
			m_IsHit = true;
			m_pAnimator->SetTrigger("Death");
			if (m_Lives > 0)
			{
				m_Lives--;
				m_pSubject->Notify(m_pOwner, Event::LivesUpdate);
			}
		}
		else if (playerEnemy) // Versus specific code
		{
			if (!otherFixture->IsSensor())
			{
				if (playerEnemy->GetIsBubbled())
				{
					playerEnemy->m_pMaitaAnimator->SetTrigger("Death");
					playerEnemy->m_IsHit = true;
					if (playerEnemy->m_Lives > 0)
					{
						playerEnemy->m_Lives--;
						playerEnemy->m_pSubject->Notify(playerEnemy->GetGameObject(), Event::LivesUpdate);
					}
					playerEnemy->m_IsBubbled = false;
				}
				else if (!m_IsHit)
				{
					// make sure player gets taken out of the physics emulation, play death animation, remove 1 life
					m_IsHit = true;
					m_pAnimator->SetTrigger("Death");
					if (m_Lives > 0)
					{
						m_Lives--;
						m_pSubject->Notify(m_pOwner, Event::LivesUpdate);
					}
				}
			}
		}
	}
}

void PlayerBehaviour::OnContactEnd(b2Contact* pContact, Box2DComponent* pThisCollider, Box2DComponent* pOther)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(pContact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(pContact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == pThisCollider)
		fixture = pContact->GetFixtureA();
	else
		fixture = pContact->GetFixtureB();
	if (fixture->IsSensor() && (pOther->GetGameObject()->GetTag() == "TileMap" || pOther->GetGameObject()->GetTag() == "LevelEdge"))
		DecrementFootCounter();

	if (!fixture->IsSensor() && pOther->GetGameObject()->GetTag() == "TileMap")
	{
		m_NrOfOverlappers--;
	}
}

// used for dropping down through platforms as longs as the down button is pressed
void PlayerBehaviour::PreSolve(b2Contact* pContact, const b2Manifold* pManifold, Box2DComponent* pThisCollider, Box2DComponent* pOther)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(pContact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(pContact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == pThisCollider)
		fixture = pContact->GetFixtureA();
	else
		fixture = pContact->GetFixtureB();
	if (!fixture->IsSensor() && pOther->GetGameObject()->GetTag() == "TileMap" && m_IsDropping)
	{
		pContact->SetEnabled(false);
	}
}

// used for one way platform (jumping through them)
bool PlayerBehaviour::ShouldCollide(b2Fixture* pFixtureA, b2Fixture* pFixtureB, Box2DComponent* pThisCollider, Box2DComponent* pOther)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(pFixtureA->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(pFixtureB->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == pThisCollider)
		fixture = pFixtureA;
	else
		fixture = pFixtureB;
	if (!fixture->IsSensor()) // ignore the foot sensor for this
	{
		if (m_IgnoreCollisions && pOther->GetGameObject()->GetTag() == "TileMap")
			return false;
	}

	return true;
}

void PlayerBehaviour::Kill()
{
	m_pOwner->SetIsEnabled(false);
	m_pBox2D->SetIsEnabled(false);
}

void PlayerBehaviour::Reset()
{
	m_pOwner->SetIsEnabled(true);
	m_pBox2D->SetIsEnabled(true);
	m_pAnimator->ResetAnimator();
	m_pMaitaAnimator->ResetAnimator();
	m_IsHit = false;
	m_IsBubbled = false;
	m_AttackTimer = m_AttackCooldown;
}

void PlayerBehaviour::SetIsMaita(bool value)
{
	m_IsMaita = value;
	if (m_IsMaita)
	{
		m_pAnimator->SetIsEnabled(false);
		m_pMaitaAnimator->SetIsEnabled(true);
		m_pRenderComp->SetTexture("Resources/enemySheet.png");
		m_pOwner->SetTag("Enemy");
	}
	else
	{
		m_pAnimator->SetIsEnabled(true);
		m_pMaitaAnimator->SetIsEnabled(false);
		m_pRenderComp->SetTexture("Resources/Player2.png");
		m_pOwner->SetTag("Player");
	}
}

void PlayerBehaviour::Hit()
{
	if (!m_IsMaita)
		return;
	m_pMaitaAnimator->SetTrigger("Bubbled");
	m_BubbledTimer = 0.f;
	m_IsBubbled = true;
}

void PlayerBehaviour::SetLives(unsigned int value)
{
	m_Lives = value; m_pSubject->Notify(m_pOwner, Event::LivesUpdate);
}
void PlayerBehaviour::SetScore(unsigned int value)
{ 
	m_Score = value; m_pSubject->Notify(m_pOwner, Event::ScoreUpdate);
}