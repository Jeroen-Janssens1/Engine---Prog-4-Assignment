#pragma once
#include "BaseComponent.h"

class TransformComponent;
class Box2DComponent : public BaseComponent
{
public:
	Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height, 
		float friction = 0.f, float density = 0.f, bool isDynamic = false, bool isKinematic = false);
	virtual ~Box2DComponent() = default;

	void Update() override;
	void Render() const override;

	//b2Body* GetPhysicsBody() const { return m_pBody; }

	void SetPosition(float x, float y);

	void SetVelocity(float x, float y);

	void GetVelocity(b2Vec2& velocity) const { velocity = m_pBody->GetLinearVelocity(); };

	void ApplyForce(const b2Vec2& force);

private:
	TransformComponent* m_pTransformParent;
	b2Body* m_pBody;

};
