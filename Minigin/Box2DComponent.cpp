#include "MiniginPCH.h"
#include "Box2DComponent.h"
#include "TransformComponent.h"
#include "Services.h"

Box2DComponent::Box2DComponent(GameObject* pOwner, TransformComponent* pTransform, b2World* pPhysicsWorld, float width, float height,
	const std::string& tag, float friction, float density, bool fixedRotation, b2Vec2 posOffset, bool isSensor, bool isDynamic, bool isKinematic,
	bool usesGravity)
	:BaseComponent(pOwner, tag)
	,m_pTransformParent{pTransform}
	,m_CollisionCallbackScript{nullptr}
{
	auto& physicVars = PhysicsVariablesService;
	// create the physics object (32 pixels = 1 unit)
	width /= physicVars.GetPixelsPerUnit();
	height /= physicVars.GetPixelsPerUnit();

	posOffset.x /= physicVars.GetPixelsPerUnit();
	posOffset.y /= physicVars.GetPixelsPerUnit();
	b2BodyDef bodyDef;
	if (isDynamic)
		bodyDef.type = b2_dynamicBody;
	if (isKinematic)
		bodyDef.type = b2_kinematicBody;
	
	if(!usesGravity)
		bodyDef.gravityScale = 0.f;
	bodyDef.fixedRotation = fixedRotation;
	bodyDef.position.Set(pTransform->GetPosition().x / physicVars.GetPixelsPerUnit(),
		pTransform->GetPosition().y / physicVars.GetPixelsPerUnit());
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

Box2DComponent::~Box2DComponent()
{
	m_CollisionCallbackScript = nullptr;

	m_pBody->GetWorld()->DestroyBody(m_pBody);
}

void Box2DComponent::Update()
{
	auto& physicsVars = PhysicsVariablesService;
	// Update the transform component so that it holds the correct position for this object
	b2Transform transform = m_pBody->GetTransform();
	float x = transform.p.x * physicsVars.GetPixelsPerUnit();
	float y = transform.p.y * physicsVars.GetPixelsPerUnit();

	m_pTransformParent->SetPosition(x, y, m_pTransformParent->GetPosition().z);
}

void Box2DComponent::Render() const
{
}

b2Fixture* Box2DComponent::AddFixture(float width, float height, float friction, float density, b2Vec2 posOffset, bool isSensor)
{
	auto& physicsVars = PhysicsVariablesService;
	posOffset.x /= physicsVars.GetPixelsPerUnit();
	posOffset.y /= physicsVars.GetPixelsPerUnit();
	width /= physicsVars.GetPixelsPerUnit();
	height /= physicsVars.GetPixelsPerUnit();

	b2PolygonShape boxShape;
	boxShape.SetAsBox(width / 2, height / 2, posOffset, 0.f);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &boxShape;
	fixtureDef.density = density;
	fixtureDef.friction = friction;
	fixtureDef.isSensor = isSensor;
	auto* fixture = m_pBody->CreateFixture(&fixtureDef);
	fixture->SetUserData(this);
	return fixture;
}

void Box2DComponent::SetPosition(float x, float y)
{
	auto& physicVars = PhysicsVariablesService;
	// convert from pixels to units
	x /= physicVars.GetPixelsPerUnit();
	y /= physicVars.GetPixelsPerUnit();
	// Set the position of the physics body
	m_pBody->SetTransform(b2Vec2(x, y), 0.f);
}

void Box2DComponent::SetVelocity(float x, float y)
{
	auto& physicVars = PhysicsVariablesService;
	x /= physicVars.GetPixelsPerUnit();
	y /= physicVars.GetPixelsPerUnit();
	m_pBody->SetLinearVelocity(b2Vec2(x, y));
}

void Box2DComponent::ApplyForce(const b2Vec2& force)
{
	m_pBody->ApplyForce(force, m_pBody->GetPosition(), true);
}
