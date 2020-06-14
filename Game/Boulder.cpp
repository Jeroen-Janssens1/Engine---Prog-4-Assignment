#include "Boulder.h"
#include "Services.h"
#include "GameObject.h"
#include "ZenBehaviour.h"
#include "PlayerBehaviour.h"

Boulder::Boulder(GameObject* pOwner, float velocity, Box2DComponent* box2D)
	:BaseComponent(pOwner)
	, m_Lifetime(2.f)
	, m_Velocity{ velocity }
	, m_pBox2D{ box2D }
	, m_HasHit{ false }
{
}

void Boulder::Update()
{
	m_pBox2D->SetVelocity(m_Velocity, 0);
	m_Lifetime -= GameTimeService.GetElapsed();
	if (m_Lifetime <= 0)
	{
		m_pOwner->Destroy();
	}
}

void Boulder::OnContactBegin(b2Contact* pContact, Box2DComponent* pThisCollider, Box2DComponent* pOther)
{
	if (pOther->GetGameObject()->GetTag() != "Enemy" && pOther->GetGameObject()->GetTag() != "Bubble")
		m_pOwner->Destroy();
}
