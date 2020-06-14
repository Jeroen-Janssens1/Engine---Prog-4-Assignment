#pragma once
#include "BaseComponent.h"
#include "GameTime.h"
#include <memory>
#include "box2d\b2_math.h"
class b2World;
class Box2DComponent;
class RenderComponent;
class SpriteAnimatorComponent;
class TransformComponent;
class SubjectComponent;
class GameObject;
class InputManager;
class PlayerBehaviour :
	public BaseComponent
{
public:
	PlayerBehaviour(GameObject* pOwner);
	~PlayerBehaviour() = default;

	void Initialize(InputManager * pInput, b2World * pPhysicsWorld, unsigned int controllerIndx, float xPos, float yPos, unsigned int lives = 4,
		unsigned int score = 0, bool isMaita = false);
	void Update() override;
	void Render() const override {};
	void OnLoad() override;

	void OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other) override;
	void OnContactEnd(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other) override;
	void PreSolve(b2Contact* contact, const b2Manifold* manifold, Box2DComponent* thisCollider, Box2DComponent* other) override;
	bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB, Box2DComponent* thisCollider, Box2DComponent* other) override;

	unsigned int GetLives() const { return m_Lives; }
	unsigned int GetScore() const { return m_Score; }
	void SetLives(unsigned int value);
	void SetScore(unsigned int value);
	SubjectComponent* GetSubject() const { return m_pSubject; }
	bool GetIsBubbled() const { return m_IsBubbled; }
	void SetIsMaita(bool value);

	void Hit();
	void Kill();
	void Reset();

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
	void ThrowBoulder();

	TransformComponent* m_pTransform;
	InputManager* m_pInput;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;
	Box2DComponent* m_pFootSensor;
	SpriteAnimatorComponent* m_pAnimator;
	SpriteAnimatorComponent* m_pMaitaAnimator;
	RenderComponent* m_pRenderComp;
	SubjectComponent* m_pSubject;
	b2Vec2 m_Vel;
	b2Vec2 m_JumpForce;
	b2Vec2 m_SpawnPos;
	const float m_Speed = 200.f;
	unsigned int m_FootSensorCounter;
	unsigned int m_NrOfOverlappers;
	bool m_IgnoreCollisions;
	bool m_IsDropping;
	bool m_IsHit;
	unsigned int m_Lives;
	const float m_AttackCooldown = 1.f;
	float m_AttackTimer;
	unsigned int m_Score;
	bool m_IsMaita;
	float m_BubbledTimer;
	const float m_BubbledDuration = 5.f;
	bool m_IsBubbled;
};

