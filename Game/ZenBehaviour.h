#pragma once
#include "BaseComponent.h"
#include "GameTime.h"
#include <memory>
#include "box2d\b2_math.h"

class GameObject;
class b2Fixture;
class ZenBehaviour :
	public BaseComponent
{
public:
	ZenBehaviour(GameObject* pOwner, unsigned int score = 100);
	~ZenBehaviour() = default;

	virtual void Initialize(b2World * pPhysicsWorld, float xPos, float yPos, TransformComponent* pPlayer = nullptr, TransformComponent* pPlayer2 = nullptr);
	void Update() override;
	void Render() const override {};
	void OnLoad() override;

	void Hit();
	void Kill();
	void Reset();
	void Set2PlayerMode(bool value) { m_2Players = value; }

	bool GetIsBubbled() const { return m_IsBubbled; }

	void OnContactBegin(b2Contact * pContact, Box2DComponent * pThisCollider, Box2DComponent * pOther) override;
	void OnContactEnd(b2Contact * pContact, Box2DComponent * pThisCollider, Box2DComponent * pTther) override;
	void PreSolve(b2Contact * pContact, const b2Manifold * pManifold, Box2DComponent * pThisCollider, Box2DComponent * pOther) override;
	bool ShouldCollide(b2Fixture * pFixtureA, b2Fixture * pFixtureB, Box2DComponent * pThisCollider, Box2DComponent * pOther) override;

	unsigned int GetScore() const { return m_Score; }

protected:
	bool MoveUp();
	bool MoveDown();
	bool MoveRight();
	bool MoveLeft();

	TransformComponent* m_pTransform;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;
	Box2DComponent* m_pFootSensor;
	SpriteAnimatorComponent* m_pAnimator;
	RenderComponent* m_pRenderComp;
	b2Fixture* m_pFootSensor;
	b2Fixture* m_pLeftSensor;
	b2Fixture* m_pRightSensor;
	b2Fixture* m_pTopSensor;
	b2Vec2 m_Vel;
	b2Vec2 m_JumpForce;
	const float m_Speed = 100.f;
	unsigned int m_FootSensorCounter;
	unsigned int m_LeftSensorCounter;
	unsigned int m_RightSensorCounter;
	unsigned int m_TopSensorCounter;
	unsigned int m_NrOfOverlappers;
	bool m_IgnoreCollisions;
	bool m_IsDropping;
	bool m_IsKilled;
	float m_BubbledTimer;
	const float m_BubbledDuration = 5.f;
	bool m_IsBubbled;
	const float m_JumpCooldown = 3.f;
	float m_JumpTimer;
	const unsigned int m_Score;
	TransformComponent* m_pPlayerTransform;
	TransformComponent* m_pPlayer2Transform;
	b2Vec2 m_SpawnPos;
	bool m_2Players;
};

