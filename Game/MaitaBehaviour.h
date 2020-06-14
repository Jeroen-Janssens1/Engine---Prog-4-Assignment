#pragma once
#include "ZenBehaviour.h"
class GameObject;
class MaitaBehaviour :
	public ZenBehaviour
{
public:
	MaitaBehaviour(GameObject* pOwner);
	~MaitaBehaviour() = default;

	void Initialize(b2World* pPhysicsWorld, float xPos, float yPos, TransformComponent* pPlayer = nullptr, TransformComponent* pPlayer2 = nullptr) override;
	void Update() override;

private:
	void ThrowBoulder();

	const float m_AttackRange = 150.f;
	const float m_AttackCooldown = 3.f;
	float m_AttackTimer;
};

