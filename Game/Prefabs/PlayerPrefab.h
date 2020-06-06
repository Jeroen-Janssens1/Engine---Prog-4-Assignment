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
class PlayerPrefab final :
	public GameObject
{
public:
	PlayerPrefab();
	~PlayerPrefab() = default;
	
	void Initialize(b2World* pPhysicsWorld, unsigned int controllerIndx);
	void Update() override;



	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

private:
	TransformComponent* m_Transform;
	InputManager& m_Input;
	GameTime& m_GameTime;
	Box2DComponent* m_pBox2D;

	b2Vec2 m_Vel;

	const float m_Speed = 10000.f;
};