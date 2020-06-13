#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include "GameTime.h"
#include "GameObject.h"
#include <memory>
#include "Command.h"
#include "box2d\b2_math.h"

class ZenBehaviour :
	public BaseComponent
{
public:
	ZenBehaviour(GameObject* pOwner);
	~ZenBehaviour() = default;

	void Initialize(b2World * pPhysicsWorld, float xPos, float yPos);
	void Update() override;
	void Render() const override {};
	void OnLoad() override {};

	void Hit();
	void Kill();

	bool GetIsBubbled() const { return m_IsBubbled; }

	//void OnContactBegin(b2Contact * contact, Box2DComponent * thisCollider, Box2DComponent * other) override;
	//void OnContactEnd(b2Contact * contact, Box2DComponent * thisCollider, Box2DComponent * other) override;
	//void PreSolve(b2Contact * contact, const b2Manifold * manifold, Box2DComponent * thisCollider, Box2DComponent * other) override;
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse, Box2DComponent* thisCollider, Box2DComponent* other) override;
	//bool ShouldCollide(b2Fixture * fixtureA, b2Fixture * fixtureB, Box2DComponent * thisCollider, Box2DComponent * other) override;

	void IncrementFootCounter() { m_FootSensorCounter++; };
	void DecrementFootCounter() { m_FootSensorCounter--; if (m_FootSensorCounter < 0) m_FootSensorCounter = 0; };

	//void MoveUp();
	//void MoveDown();
	//void MoveRight();
	//void MoveLeft();

private:
	TransformComponent* m_pTransform;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;
	Box2DComponent* m_pFootSensor;
	SpriteAnimatorComponent* m_pAnimator;
	RenderComponent* m_pRenderComp;

	b2Vec2 m_Vel;
	b2Vec2 m_JumpForce;

	const float m_Speed = 10000.f;

	unsigned int m_FootSensorCounter;

	unsigned int m_NrOfOverlappers;
	bool m_IgnoreCollisions;
	bool m_IsDropping;
	bool m_IsKilled;

	float m_BubbledTimer;
	const float m_BubbledDuration = 5.f;
	bool m_IsBubbled;
};

