#include "MiniginPCH.h"
#include "Box2DComponent.h"
#include "TransformComponent.h"

Box2DComponent::Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height,
	float friction, float density, bool isDynamic, bool isKinematic)
	:BaseComponent(pOwner)
	,m_pTransformParent{pTransform}
{
	// create the physics object (32 pixels = 1 unit)
	width /= 16;
	height /= 16;
	b2BodyDef bodyDef;
	if (isDynamic)
		bodyDef.type = b2_dynamicBody;
	if (isKinematic)
		bodyDef.type = b2_kinematicBody;
	
	bodyDef.position.Set(pTransform->GetPosition().x / 16, pTransform->GetPosition().y / 16); // 0, 14

	m_pBody = pPhysicsWorld->CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	m_pBody->CreateFixture(&fixtureDef);

	
}

void Box2DComponent::Update()
{
	// Update the transform component so that it holds the correct position for this object
	b2Transform transform = m_pBody->GetTransform();
	float x = transform.p.x * 16;
	float y = transform.p.y * 16;
	m_pTransformParent->SetPosition(x, y, m_pTransformParent->GetPosition().z);
}

void Box2DComponent::Render() const
{
}

void Box2DComponent::SetPosition(float x, float y)
{
	// convert from pixels to units
	x /= 16;
	y /= 16;
	// Set the position of the physics body
	m_pBody->SetTransform(b2Vec2(x, y), 0.f);
}

void Box2DComponent::SetVelocity(float x, float y)
{
	m_pBody->SetLinearVelocity(b2Vec2(x, y));
}

void Box2DComponent::ApplyForce(const b2Vec2& force)
{
	m_pBody->ApplyForce(force, m_pBody->GetPosition(), true);
}
