#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(std::shared_ptr<GameObject>& pOwner, std::shared_ptr<TransformComponent>& pTransform)
	:BaseComponent{pOwner}
	,m_pTransformParent{pTransform}
	,m_Texture{}
{

}

void RenderComponent::Update()
{

}

void RenderComponent::Render() const
{
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, m_pTransformParent.lock()->GetPosition().x, m_pTransformParent.lock()->GetPosition().y);
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}