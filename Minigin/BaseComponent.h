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
	

private:
	GameObject* m_pOwner;
};

