#pragma once
#include "BaseComponent.h"
class Box2DComponent;
class Boulder :
	public BaseComponent
{
public:
	Boulder(GameObject* pOwner, float velocity, Box2DComponent* box2D);
	void Update() override;
	void Render() const override {};
	void OnLoad() override {};

	void OnContactBegin(b2Contact* contact, Box2DComponent* thisCollider, Box2DComponent* other) override;

private:
	float m_Lifetime;
	float m_Velocity;
	Box2DComponent* m_Box2D;
	bool m_HasHit;
};

