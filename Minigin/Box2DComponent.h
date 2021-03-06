#pragma once
#include "BaseComponent.h"
#include "Box2D.h"
class b2Fixture;
class PhysicsVariables;
class TransformComponent;
class Box2DComponent final : public BaseComponent
{
public:
	Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height, 
		const std::string& tag = "", float friction = 0.f, float density = 0.f, bool fixedRotation = true, b2Vec2 posOffset = b2Vec2(0.f, 0.f), 
		bool isSensor = false, bool isDynamic = false, bool isKinematic = false, bool usesGravity = true);
	virtual ~Box2DComponent();

	Box2DComponent(const Box2DComponent& other) = delete;
	Box2DComponent(Box2DComponent&& other) = delete;
	Box2DComponent& operator=(const Box2DComponent& other) = delete;
	Box2DComponent& operator=(Box2DComponent&& other) = delete;

	void Update() override;
	void Render() const override {};
	void OnLoad() override {};

	b2Fixture* AddFixture(float width, float height, float friction = 0.f, float density = 0.f, b2Vec2 posOffset = b2Vec2(0.f, 0.f), bool isSensor = false);

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
	void SetCollisionCallbackScript(BaseComponent* ccs) { m_CollisionCallbackScript = ccs; }
	BaseComponent* GetCollisionCallbackScript() const { return m_CollisionCallbackScript; }

private:
	TransformComponent* m_pTransformParent;
	b2Body* m_pBody;
	BaseComponent* m_CollisionCallbackScript;
};

