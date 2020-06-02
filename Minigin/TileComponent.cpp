#include "MiniginPCH.h"
#include "TileComponent.h"
#include "RenderComponent.h"

TileComponent::TileComponent(int xPos, int yPos, RenderComponent* rendererReference, TransformComponent* transformReference,
	GameObject* pOwner, const std::string& texturePath)
	:BaseComponent(pOwner)
	,m_XPos{xPos}
	,m_YPos{yPos}
{
	m_RendererReference = rendererReference;
	m_TransformReference = transformReference;
	rendererReference->SetTexture(texturePath);
}

void TileComponent::Update()
{
	
}

void TileComponent::Render() const
{
}

int TileComponent::GetXPos()
{
	return m_XPos;
}

int TileComponent::GetYPos()
{
	return m_YPos;
}
