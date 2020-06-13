#pragma once
#include <memory>
#include <string>
class GameObject;
class b2Contact;
class Box2DComponent;
class b2Fixture;
struct b2Manifold;
struct b2ContactImpulse;
class BaseComponent
{
public:
	BaseComponent(GameObject* pOwner, const std::string& tag = "");
	virtual ~BaseComponent() = default;
	
	virtual void Update()=0;
	virtual void Render() const = 0;
	virtual void OnLoad() = 0;

	// for collision callback functionality within custom scripts/components (a little like in Unity's OnTriggerEnter and Exit functions
	virtual void OnContactBegin(b2Contact*, Box2DComponent*, Box2DComponent*) { };
	virtual void OnContactEnd(b2Contact*, Box2DComponent*, Box2DComponent*) { };
	virtual void PreSolve(b2Contact*, const b2Manifold*, Box2DComponent*, Box2DComponent*) {};
	virtual void PostSolve(b2Contact*, const b2ContactImpulse*, Box2DComponent*, Box2DComponent*) {};
	virtual bool ShouldCollide(b2Fixture*, b2Fixture*, Box2DComponent*, Box2DComponent*) { return true; };

	GameObject* GetGameObject() const { return m_pOwner; }

	const std::string& GetTag() const { return m_Tag; }

	virtual void SetIsEnabled(bool value) { m_IsEnabled = value; };
	bool GetIsEnabled() const { return m_IsEnabled; };

protected:
	GameObject* m_pOwner;
	bool m_IsEnabled;
	std::string m_Tag;
};

