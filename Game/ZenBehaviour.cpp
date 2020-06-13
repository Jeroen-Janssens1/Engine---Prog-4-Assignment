#include "ZenBehaviour.h"
#include "Services.h"
#include "AnimatorIncludes.h"

ZenBehaviour::ZenBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	,m_GameTime(GameTimeService)
	,m_pPlayerTransform{nullptr}
	,m_pPlayer2Transform{nullptr}
	,m_Score{100}
	,m_2Players{false}
{
}

void ZenBehaviour::Initialize(b2World* pPhysicsWorld, float xPos, float yPos, TransformComponent* pPlayer, TransformComponent* pPlayer2)
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
	int nrOfAnimations = 2;
	std::vector<Animation*> animations{};
	b2Vec2 framePos{};
	framePos.x = 0;
	framePos.y = 0;
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
		
		// for this you first need to set up the frame positions
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
	m_pAnimator = new SpriteAnimatorComponent(m_pOwner, animations);
	m_pOwner->AddComponent(m_pAnimator);

	m_pTransform->SetPosition(xPos, yPos, 0);

	m_pBox2D = new Box2DComponent(m_pOwner, m_pTransform, pPhysicsWorld,
		m_pRenderComp->GetWidth() - 10.f, m_pRenderComp->GetHeight(), "FootSensor", 1.f, 1.f, true, b2Vec2(0.f, 0.f), false, true);
	m_pOwner->AddComponent(m_pBox2D);

	// add sensors to this body
	m_FootSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_LeftSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(-m_pRenderComp->GetWidth()/2.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_RightSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(m_pRenderComp->GetWidth() / 2.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);
	m_TopSensor = m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (-m_pRenderComp->GetHeight() / 2.f) - 32.f), true);
	
	m_pBox2D->SetCollisionCallbackScript(this);
}

void ZenBehaviour::Update()
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
	// check player posses first
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
			// if there are 2 players, check which one is closer
			if (abs(heightDif2) < abs(heightDif) || (abs(heightDif2) == abs(heightDif) && abs(xDif2) < abs(xDif)) || !m_pPlayerTransform->GetGameObject()->GetIsEnabled())
			{
				// track player 2
				// check if we should move up
				if (heightDif2 < 0)
				{
					// move up
					if (!MoveUp())
					{
						if (xDif2 > 0) // move right
						{
							MoveRight();
						}
						else // move left
						{
							MoveLeft();
						}
					}
				}
				else if (heightDif2 > 0) // move down
				{
					// move down
					MoveDown();
				}
				else // move right or left
				{
					if (xDif2 > 0) // move right
					{
						MoveRight();
					}
					else // move left
					{
						MoveLeft();
					}
				}
			}
			else
			{
				// track player 1
				// check if we should move up
				if (heightDif < 0)
				{
					// move up
					if (!MoveUp())
					{
						if (xDif > 0) // move right
						{
							MoveRight();
						}
						else // move left
						{
							MoveLeft();
						}
					}
				}
				else if (heightDif > 0) // move down
				{
					// move down
					MoveDown();
				}
				else // move right or left
				{
					if (xDif > 0) // move right
					{
						MoveRight();
					}
					else // move left
					{
						MoveLeft();
					}
				}
			}
		}
		else
		{
			// track player 1
				// check if we should move up
			if (heightDif < 0)
			{
				// move up
				if (!MoveUp())
				{
					if (xDif > 0) // move right
					{
						MoveRight();
					}
					else // move left
					{
						MoveLeft();
					}
				}
			}
			else if (heightDif > 0) // move down
			{
				// move down
				MoveDown();
			}
			else // move right or left
			{
				if (xDif > 0) // move right
				{
					MoveRight();
				}
				else // move left
				{
					MoveLeft();
				}
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

		if (m_JumpTimer < m_JumpCooldown)
			m_JumpTimer += m_GameTime.GetElapsed();
	}
}

void ZenBehaviour::OnLoad()
{
	m_pBox2D->SetPosition(m_SpawnPos.x, m_SpawnPos.y);
}

void ZenBehaviour::Hit()
{
	m_pAnimator->SetTrigger("Bubbled");
	m_BubbledTimer = 0.f;
	m_IsBubbled = true;
}

void ZenBehaviour::Kill()
{
	m_IsKilled = true;
}

void ZenBehaviour::Reset()
{
	m_pOwner->SetIsEnabled(true);
	m_pBox2D->SetIsEnabled(true);
	m_IsKilled = false;
	m_pAnimator->ResetAnimator();
	m_IsBubbled = false;
}


void ZenBehaviour::OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = contact->GetFixtureA();
	else
		fixture = contact->GetFixtureB();
	if (other->GetGameObject()->GetTag() == "TileMap" || other->GetGameObject()->GetTag() == "LevelEdge")
	{
		if (fixture == m_FootSensor)
			m_FootSensorCounter++;
		if (fixture == m_RightSensor)
			m_RightSensorCounter++;
		if (fixture == m_LeftSensor)
			m_LeftSensorCounter++;
		if (fixture == m_TopSensor)
			m_TopSensorCounter++;
	}
	if (!fixture->IsSensor() && other->GetGameObject()->GetTag() == "TileMap")
	{
		m_NrOfOverlappers++;
	}
}

void ZenBehaviour::OnContactEnd(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = contact->GetFixtureA();
	else
		fixture = contact->GetFixtureB();
	if (other->GetGameObject()->GetTag() == "TileMap" || other->GetGameObject()->GetTag() == "LevelEdge")
	{
		if (fixture == m_FootSensor)
			m_FootSensorCounter--;
		if (fixture == m_RightSensor)
			m_RightSensorCounter--;
		if (fixture == m_LeftSensor)
			m_LeftSensorCounter--;
		if (fixture == m_TopSensor)
			m_TopSensorCounter--;
	}
	if (!fixture->IsSensor() && other->GetGameObject()->GetTag() == "TileMap")
	{
		m_NrOfOverlappers--;
	}
}

// used for dropping down through platforms as longs as the down button is pressed
void ZenBehaviour::PreSolve(b2Contact* contact, const b2Manifold* manifold, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = contact->GetFixtureA();
	else
		fixture = contact->GetFixtureB();
	if (!fixture->IsSensor() && other->GetGameObject()->GetTag() == "TileMap" && m_IsDropping)
	{
		contact->SetEnabled(false);
	}
}

// used for one way platform (jumping through them)
bool ZenBehaviour::ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB, Box2DComponent* thisCollider, Box2DComponent* other)
{
	Box2DComponent* collider1 = static_cast<Box2DComponent*>(fixtureA->GetUserData());
	Box2DComponent* collider2 = static_cast<Box2DComponent*>(fixtureB->GetUserData());
	b2Fixture* fixture = nullptr;
	if (collider1 == thisCollider)
		fixture = fixtureA;
	else
		fixture = fixtureB;

	if (!fixture->IsSensor()) // ignore the foot sensor for this
	{
		if (m_IgnoreCollisions && other->GetGameObject()->GetTag() == "TileMap")
			return false;
		if (other->GetGameObject()->GetTag() == "Enemy")
			return false;
	}

	return true;
}

bool ZenBehaviour::MoveUp()
{
	// check if we can move up
	if (m_JumpTimer < m_JumpCooldown)
		return false;
	b2Vec2 vel;
	m_pBox2D->GetVelocity(vel);
	if (m_TopSensorCounter == 0 && vel.y > 0)
		return false;
	
	if (m_FootSensorCounter != 0)
	{
		m_JumpForce = b2Vec2(0.f, -300.f);
		m_JumpTimer = 0;
		m_IgnoreCollisions = true;
	}
	return true;
}

bool ZenBehaviour::MoveDown()
{
	m_IsDropping = true;
	m_IgnoreCollisions = true;
	m_JumpTimer = 0;
	return true;
}

bool ZenBehaviour::MoveRight()
{
	if (m_RightSensorCounter == 0)
		return false;
	m_Vel.x += m_Speed;
	m_pRenderComp->SetIsFlipped(true);
	return true;
}

bool ZenBehaviour::MoveLeft()
{
	if (m_LeftSensorCounter == 0)
		return false;
	m_Vel.x += -m_Speed;
	m_pRenderComp->SetIsFlipped(false);
	return true;
}
