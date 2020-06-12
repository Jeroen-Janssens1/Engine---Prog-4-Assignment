#pragma once
#include <memory>
#include <string>
class GameObject;
class b2Contact;
class Box2DComponent;
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

	GameObject* GetGameObject() const { return m_pOwner; }

	const std::string& GetTag() const { return m_Tag; }

	virtual void SetIsEnabled(bool value) { m_IsEnabled = value; };
	bool GetIsEnabled() const { return m_IsEnabled; };

protected:
	GameObject* m_pOwner;
	bool m_IsEnabled;
	std::string m_Tag;
};

