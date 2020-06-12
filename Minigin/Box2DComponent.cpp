#include "MiniginPCH.h"
#include "Box2DComponent.h"
#include "TransformComponent.h"
#include "Services.h"

Box2DComponent::Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height,
	const std::string& tag, float friction, float density, bool fixedRotation, b2Vec2 posOffset, bool isSensor, bool isDynamic, bool isKinematic)
	:BaseComponent(pOwner, tag)
	,m_pTransformParent{pTransform}
	,m_CollisionCallbackScript{nullptr}
{
	m_PhysicVars = PhysicsVariablesService;
	// create the physics object (32 pixels = 1 unit)
	width /= m_PhysicVars.GetPixelsPerUnit();
	height /= m_PhysicVars.GetPixelsPerUnit();

	posOffset.x /= m_PhysicVars.GetPixelsPerUnit();
	posOffset.y /= m_PhysicVars.GetPixelsPerUnit();
	b2BodyDef bodyDef;
	if (isDynamic)
		bodyDef.type = b2_dynamicBody;
	if (isKinematic)
		bodyDef.type = b2_kinematicBody;
	
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.position.Set(pTransform->GetPosition().x / m_PhysicVars.GetPixelsPerUnit(),
		pTransform->GetPosition().y / m_PhysicVars.GetPixelsPerUnit());
	m_pBody = pPhysicsWorld->CreateBody(&bodyDef);
	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2, posOffset, 0.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;
	m_pBody->CreateFixture(&fixtureDef)->SetUserData(this);

	
}

void Box2DComponent::Update()
{
	// Update the transform component so that it holds the correct position for this object
	b2Transform transform = m_pBody->GetTransform();
	float x = transform.p.x * m_PhysicVars.GetPixelsPerUnit();
	float y = transform.p.y * m_PhysicVars.GetPixelsPerUnit();

	m_pTransformParent->SetPosition(x, y, m_pTransformParent->GetPosition().z);
}

void Box2DComponent::Render() const
{
}

void Box2DComponent::AddFixture(float width, float height, float friction, float density, b2Vec2 posOffset, bool isSensor)
{
	posOffset.x /= m_PhysicVars.GetPixelsPerUnit();
	posOffset.y /= m_PhysicVars.GetPixelsPerUnit();
	width /= m_PhysicVars.GetPixelsPerUnit();
	height /= m_PhysicVars.GetPixelsPerUnit();

	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2, posOffset, 0.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;
	m_pBody->CreateFixture(&fixtureDef)->SetUserData(this);
}

void Box2DComponent::SetPosition(float x, float y)
{
	// convert from pixels to units
	x /= m_PhysicVars.GetPixelsPerUnit();
	y /= m_PhysicVars.GetPixelsPerUnit();
	// Set the position of the physics body
	m_pBody->SetTransform(b2Vec2(x, y), 0.f);
}

void Box2DComponent::SetVelocity(float x, float y)
{
	x /= m_PhysicVars.GetPixelsPerUnit();
	y /= m_PhysicVars.GetPixelsPerUnit();
	m_pBody->SetLinearVelocity(b2Vec2(x, y));
}

void Box2DComponent::ApplyForce(const b2Vec2& force)
{
	m_pBody->ApplyForce(force, m_pBody->GetPosition(), true);
}
