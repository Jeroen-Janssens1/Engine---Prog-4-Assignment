#include "MiniginPCH.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

RenderComponent::RenderComponent(std::shared_ptr<GameObject>& pOwner, std::shared_ptr<TransformComponent>& pTransform,
	float width, float height, bool isSpriteSheet,
	int nrCols, int nrRows, int cellWidth, int cellHeight, int xPos, int yPos)
	:BaseComponent{pOwner}
	,m_pTransformParent{pTransform}
	,m_Texture{}
	,m_IsSpriteSheet{isSpriteSheet}
	,m_NrCols{nrCols}
	,m_NrRows{nrRows}
	,m_CellWidth{cellWidth}
	,m_CellHeight{cellHeight}
	,m_XPos{xPos}
	,m_YPos{yPos}
	,m_Width{width}
	,m_Height{height}
{
	
}

void RenderComponent::Update()
{

}

void RenderComponent::Render() const
{
	if (m_IsSpriteSheet)
	{
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, m_pTransformParent.lock()->GetPosition().x, m_pTransformParent.lock()->GetPosition().y,
			m_XPos, m_YPos, m_CellWidth, m_CellHeight, m_Width, m_Height);
		return;
	}
	dae::Renderer::GetInstance().RenderTexture(*m_Texture, m_pTransformParent.lock()->GetPosition().x, m_pTransformParent.lock()->GetPosition().y);
}

void RenderComponent::SetTexture(const std::string& filename)
{
	m_Texture = dae::ResourceManager::GetInstance().LoadTexture(filename);
}