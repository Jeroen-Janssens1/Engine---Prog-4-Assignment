#pragma once
#include <memory>
class GameObject;
class BaseComponent
{
public:
	BaseComponent(std::shared_ptr<GameObject>& pOwner);
	virtual ~BaseComponent() = default;
	
	virtual void Update()=0;
	virtual void Render() const = 0;
	

private:
	std::weak_ptr<GameObject> m_pOwner;
};

