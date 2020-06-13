#include "Bubble.h"
#include "Services.h"
#include "GameObject.h"
#include "ZenBehaviour.h"

Bubble::Bubble(GameObject* pOwner, float velocity, Box2DComponent* box2D)
	:BaseComponent(pOwner)
	,m_Lifetime(1.f)
	,m_Velocity{velocity}
	,m_Box2D{box2D}
	,m_HasHit{false}
{
}

void Bubble::Update()
{
	m_Box2D->SetVelocity(m_Velocity, 0);
	m_Lifetime -= GameTimeService.GetElapsed();
	if (m_Lifetime <= 0)
	{
		m_pOwner->Destroy();
	}
}

void Bubble::OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other)
{
	// check if colliding with enemy
	if (other->GetGameObject()->GetTag() == "Enemy")
	{
		ZenBehaviour* enemy = other->GetGameObject()->GetComponent<ZenBehaviour>("Enemy");
		if (enemy)
		{
			if (!enemy->GetIsBubbled() && !m_HasHit)
			{
				enemy->Hit();
				m_pOwner->Destroy(); // destroys object at start of next frame
				m_HasHit = true;
			}
		}
	}
	else if (other->GetGameObject()->GetTag() == "TileMap" || other->GetGameObject()->GetTag() == "LevelEdge")
	{
		m_pOwner->Destroy();
	}
}
