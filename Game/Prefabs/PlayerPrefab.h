#pragma once
#include "InputManager.h"
#include "GameTime.h"
#include "GameObject.h"
#include <memory>
#include "Command.h"
#include "box2d\b2_math.h"

class b2World;
class TransformComponent;
class Box2DComponent;
class SpriteAnimatorComponent;
class PlayerPrefab final :
	public GameObject
{
public:
	PlayerPrefab();
	~PlayerPrefab() = default;
	
	void Initialize(InputManager* pInput, b2World* pPhysicsWorld, unsigned int controllerIndx);
	void Update() override;



	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

private:
	TransformComponent* m_Transform;
	InputManager* m_pInput;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;
	SpriteAnimatorComponent* m_pAnimator;

	b2Vec2 m_Vel;
	b2Vec2 m_JumpForce;

	const float m_Speed = 10000.f;
};