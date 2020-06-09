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

	void Initialize(InputManager * pInput, b2World * pPhysicsWorld, unsigned int controllerIndx);
	void Update() override;
	virtual void Render() const override {};



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

