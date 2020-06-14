#pragma once
#include "BaseComponent.h"
class Box2DComponent;
class Boulder :
	public BaseComponent
{
public:
	Boulder(GameObject* pOwner, float velocity, Box2DComponent* pBox2D);
	~Boulder() = default;

	void Update() override;
	void Render() const override {};
	void OnLoad() override {};

	void OnContactBegin(b2Contact* pContact, Box2DComponent* pThisCollider, Box2DComponent* pOther) override;

private:
	float m_Lifetime;
	float m_Velocity;
	Box2DComponent* m_pBox2D;
	bool m_HasHit;
};

