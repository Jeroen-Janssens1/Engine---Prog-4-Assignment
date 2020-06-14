#include "MiniginPCH.h"
#include "TileComponent.h"
#include "RenderComponent.h"

TileComponent::TileComponent(int xPos, int yPos,
	GameObject* pOwner)
	:BaseComponent(pOwner)
	,m_XPos{xPos}
	,m_YPos{yPos}
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
