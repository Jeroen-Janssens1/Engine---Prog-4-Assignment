#include "ZenBehaviour.h"
#include "Services.h"
#include "AnimatorIncludes.h"

ZenBehaviour::ZenBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	,m_GameTime(GameTimeService)
{
}

void ZenBehaviour::Initialize(b2World* pPhysicsWorld, float xPos, float yPos)
{
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
		m_pRenderComp->GetWidth(), m_pRenderComp->GetHeight(), "FootSensor", 1.f, 1.f, true, b2Vec2(0.f, 0.f), false, true);
	m_pOwner->AddComponent(m_pBox2D);

	// add footsensor fixture to this body
	m_pBox2D->AddFixture(m_pRenderComp->GetWidth() - 16.f, 8.f, 0.f, 0.f, b2Vec2(0.f, (m_pRenderComp->GetHeight() / 2.f) + 4.f), true);

	m_pBox2D->SetCollisionCallbackScript(this);
}

void ZenBehaviour::Update()
{
	if (m_IsKilled)
	{
		m_pOwner->SetIsEnabled(false);
		m_pBox2D->SetIsEnabled(false);
	}

	if (m_pAnimator->GetCurrentStateName() == "Bubbled")
	{
		m_BubbledTimer += m_GameTime.GetElapsed();
		if (m_BubbledTimer >= m_BubbledDuration)
		{
			m_pAnimator->SetTrigger("Walking");
			m_IsBubbled = false;
		}
	}
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
