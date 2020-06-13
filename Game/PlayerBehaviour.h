#pragma once
#include "BaseComponent.h"
#include "InputManager.h"
#include "GameTime.h"
#include "GameObject.h"
#include <memory>
#include "Command.h"
#include "box2d\b2_math.h"

class PlayerBehaviour :
	public BaseComponent
{
public:
	PlayerBehaviour(GameObject* pOwner);
	~PlayerBehaviour() = default;

	void Initialize(InputManager * pInput, b2World * pPhysicsWorld, unsigned int controllerIndx, float xPos, float yPos);
	void Update() override;
	void Render() const override {};
	void OnLoad() override {};

	void OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other) override;
	void OnContactEnd(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other) override;
	void PreSolve(b2Contact* contact, const b2Manifold* manifold, Box2DComponent* thisCollider, Box2DComponent* other) override;
	//void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse, Box2DComponent* thisCollider, Box2DComponent* other) override;
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB, Box2DComponent* thisCollider, Box2DComponent* other) override;

	unsigned int GetLives() const { return m_Lives; }

	void IncrementFootCounter() { m_FootSensorCounter++; };
	void DecrementFootCounter() { m_FootSensorCounter--; if (m_FootSensorCounter < 0) m_FootSensorCounter = 0; };

	// input functions
	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();
	void Attack();

private:
	void SpawnBubble();

	TransformComponent* m_pTransform;
	InputManager* m_pInput;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;
	Box2DComponent* m_pFootSensor;
	SpriteAnimatorComponent* m_pAnimator;
	RenderComponent* m_pRenderComp;

	b2Vec2 m_Vel;
	b2Vec2 m_JumpForce;
	b2Vec2 m_SpawnPos;

	const float m_Speed = 10000.f;

	unsigned int m_FootSensorCounter;

	unsigned int m_NrOfOverlappers;
	bool m_IgnoreCollisions;
	bool m_IsDropping;
	bool m_IsHit;

	unsigned int m_Lives;

	const float m_AttackCooldown = 1.f;
	float m_AttackTimer;

};

