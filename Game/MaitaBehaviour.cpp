#include "MaitaBehaviour.h"
#include "AnimatorIncludes.h"
#include "Services.h"
#include "Boulder.h"
#include "Scene.h"

MaitaBehaviour::MaitaBehaviour(GameObject* pOwner)
	:ZenBehaviour(pOwner, 200)
{

}

void MaitaBehaviour::Initialize(b2World* pPhysicsWorld, float xPos, float yPos, TransformComponent* pPlayer, TransformComponent* pPlayer2)
{
	m_SpawnPos.x = xPos;
	m_SpawnPos.y = yPos;
	m_JumpTimer = 0;
	m_pPlayerTransform = pPlayer;
	m_pPlayer2Transform = pPlayer2;
	m_FootSensorCounter = 0;
	m_LeftSensorCounter = 0;
	m_RightSensorCounter = 0;
	m_TopSensorCounter = 0;
	m_IsBubbled = false;
	m_IsKilled = false;
	m_NrOfOverlappers = 0;
	m_IgnoreCollisions = false;
	m_pTransform = new TransformComponent(m_pOwner);
	m_pOwner->AddComponent(m_pTransform);

	m_pRenderComp = new RenderComponent(m_pOwner, m_pTransform, "", 32, 32, true, 16, 16, 0, 0);
	m_pOwner->AddComponent(m_pRenderComp);

	m_pRenderComp->SetTexture("Resources/enemySheet.png");
	m_Tag = "Enemy";


	// after setting up the render component, create all the animations you need
	// Each animation takes a vector of b2Vec2. These are used as positions on the sprite sheet, each position representing 1 frame of the animation
	int nrOfAnimations = 2;
	std::vector<Animation*> animations{};
	b2Vec2 framePos{};
	framePos.x = 0;
	framePos.y = 21;
	for (int i{}; i < nrOfAnimations; i++)
	{
		int nrOfFrames{};
		std::vector<b2Vec2> framePositions{};
		framePos.x = 0;
		if (i == 0)
			nrOfFrames = 2;
		else if (i == 1)
		{
			nrOfFrames = 2;
			framePos.x = 124;
		}
		// set the frame positions up
		for (int j{}; j < nrOfFrames; j++)
		{
			framePositions.push_back(framePos);
			framePos.x += 21;
		}
		// Then you push back the new animation
		if (i == 0)
		{
			animations.push_back(new Animation("Walking", m_pRenderComp, framePositions, 0.25f));
		}
		else if (i == 1)
			animations.push_back(new Animation("Bubbled", m_pRenderComp, framePositions, 0.3f));
	}
	// now you need to set up the transitions
	Transition* transition = new Transition(animations[1], true, false, "Bubbled");
	animations[0]->AddTransition(transition);
	transition = new Transition(animations[0], true, false, "Walking");
	animations[1]->AddTransition(transition);

	//Make the actual animator component, this component will take care of all the deletion of the animations and transitions
	//It's essentially a FSM. It just switches between Animations (States) using Transitions. The Animation class holds the actual animating functionality
	m_pAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pAnimator);

	m_pTransform->SetPosition(xPos, yPos, 0);

	m_pBox2D = new Box2DComponent(m_pOwner, m_pTransform, pPhysicsWorld,
		m_pRenderComp->GetWidth() - 10.f, m_pRenderComp->GetHeight(), "FootSensor", 1.f, 1.f, true, b2Vec2(0.f, 0.f), false, true);
	m_pOwner->AddComponent(m_pBox2D);

	// add sensors to this body for AI purposes
	m_pFootSensorFixture = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_pLeftSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(-m_pRenderComp->GetWidth() / 2.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_pRightSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(m_pRenderComp->GetWidth() / 2.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_pTopSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 20.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (-m_pRenderComp->GetHeight() / 2.f) - 32.f), true);

	m_pBox2D->SetCollisionCallbackScript(this);
}

void MaitaBehaviour::Update()
{
	if (m_IsKilled)
	{
		m_pOwner->SetIsEnabled(false);
		m_pBox2D->SetIsEnabled(false);
		return;
	}

	if (m_pAnimator->GetCurrentStateName() == "Bubbled")
	{
		m_pBox2D->SetVelocity(0.f, 0.f);
		m_BubbledTimer += m_GameTime.GetElapsed();
		if (m_BubbledTimer >= m_BubbledDuration)
		{
			m_pAnimator->SetTrigger("Walking");
			m_IsBubbled = false;
		}
		return;
	}
	m_IsDropping = false;
	// check player positions first
	if (m_pPlayerTransform)
	{
		float xDif{};
		float yDif{};
		xDif = m_pPlayerTransform->GetPosition().x - m_pTransform->GetPosition().x;
		yDif = m_pPlayerTransform->GetPosition().y - m_pTransform->GetPosition().y;
		int heightDif = int(yDif / 16); // round down
		if (m_pPlayer2Transform && m_2Players && m_pPlayer2Transform->GetGameObject()->GetIsEnabled())
		{
			// AI Behaviour

			int heightDif2 = int((m_pPlayer2Transform->GetPosition().y - m_pTransform->GetPosition().y) / 16);
			float xDif2 = m_pPlayer2Transform->GetPosition().x - m_pTransform->GetPosition().x;
			// if there are 2 players, check if player 2 is closer
			// the first one checks if they are closer in the y direction, the second one checks the x distance in case the y distance is equal, the last one checks if the first player is death or not
			if (abs(heightDif2) < abs(heightDif) || (abs(heightDif2) == abs(heightDif) && abs(xDif2) < abs(xDif)) || !m_pPlayerTransform->GetGameObject()->GetIsEnabled())
			{
				// track player 2
				if (heightDif2 < 0)
				{
					if (!MoveUp()) // move up returns false if the jump cooldown hasn't finished yet
					{
						if (xDif2 > 0)
						{
							MoveRight();
						}
						else
						{
							MoveLeft();
						}
					}
				}
				else if (heightDif2 > 0)
				{
					MoveDown();
				}
				else if(xDif2 > m_AttackRange) // move right or left or attack
				{
					if (xDif2 > 0)
					{
						MoveRight();
					}
					else
					{
						MoveLeft();
					}
				}
				else
				{
					ThrowBoulder();
				}
			}
			else
			{
				// track player 1
				if (heightDif < 0)
				{
					if (!MoveUp())
					{
						if (xDif > 0)
						{
							MoveRight();
						}
						else
						{
							MoveLeft();
						}
					}
				}
				else if (heightDif > 0)
				{
					MoveDown();
				}
				else if(xDif > m_AttackRange) // move right or left or attack
				{
					if (xDif > 0)
					{
						MoveRight();
					}
					else
					{
						MoveLeft();
					}
				}
				else
				{
					ThrowBoulder();
				}
			}
		}
		else
		{
			// track player 1
			if (heightDif < 0)
			{
				if (!MoveUp())
				{
					if (xDif > 0)
					{
						MoveRight();
					}
					else
					{
						MoveLeft();
					}
				}
			}
			else if (heightDif > 0)
			{
				MoveDown();
			}
			else if(xDif > m_AttackRange) // move right or left
			{
				if (xDif > 0)
				{
					MoveRight();
				}
				else
				{
					MoveLeft();
				}
			}
			else
			{
				ThrowBoulder();
			}
		}

		// actually move the AI
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

		// update timers if necessary
		if (m_JumpTimer < m_JumpCooldown)
			m_JumpTimer += m_GameTime.GetElapsed();

		if (m_AttackTimer < m_AttackCooldown)
			m_AttackTimer += m_GameTime.GetElapsed();
	}
}

void MaitaBehaviour::ThrowBoulder()
{
	if (m_AttackTimer < m_AttackCooldown)
		return;

	m_AttackTimer = 0;
	float spawnVel = 1;
	// spawn a boulder in the current facing direction
	if (!m_pRenderComp->GetIsFlipped()) // if not flipped, left direction
	{
		spawnVel = -1;
	}
	// spawn the actual boulder
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
