#include "MiniginPCH.h"
#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* pOwner, float x, float y, float z, const std::string& tag)
	:BaseComponent{pOwner, tag}
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