#include "MiniginPCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(std::shared_ptr<GameObject>& pOwner, float x, float y, float z)
	:BaseComponent{pOwner}
	,m_Position{x, y, z}
{
	
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}

void TransformComponent::Update()
{

}

void TransformComponent::Render() const
{

}