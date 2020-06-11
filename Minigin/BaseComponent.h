#pragma once
#include <memory>
class GameObject;
class BaseComponent
{
public:
	BaseComponent(GameObject* pOwner);
	virtual ~BaseComponent() = default;
	
	virtual void Update()=0;
	virtual void Render() const = 0;
	virtual void OnLoad() = 0;

	virtual void SetIsEnabled(bool value) { m_IsEnabled = value; };
	bool GetIsEnabled() const { return m_IsEnabled; };

protected:
	GameObject* m_pOwner;
	bool m_IsEnabled;
};

