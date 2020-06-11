#pragma once
#include "BaseComponent.h"
#include "Box2D.h"

class TransformComponent;
class Box2DComponent final : public BaseComponent
{
public:
	Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height, 
		float friction = 0.f, float density = 0.f, bool isDynamic = false, bool isKinematic = false);
	virtual ~Box2DComponent() = default;

	void Update() override;
	void Render() const override;
	void OnLoad() override {}

	//b2Body* GetPhysicsBody() const { return m_pBody; }

	void SetPosition(float x, float y);

	void SetVelocity(float x, float y);

	void GetVelocity(b2Vec2& velocity) const {
		velocity.x = (m_pBody->GetLinearVelocity().x * 32.f);
		velocity.y = (m_pBody->GetLinearVelocity().y * 32.f);
	};

	void ApplyForce(const b2Vec2& force);

	void SetIsEnabled(bool value) override { 
		m_IsEnabled = value;
		m_pBody->SetEnabled(value);
	}

private:
	TransformComponent* m_pTransformParent;
	b2Body* m_pBody;

};

