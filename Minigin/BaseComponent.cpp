#include "MiniginPCH.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent(GameObject* pOwner, const std::string& tag)
	:m_pOwner{pOwner}
	,m_IsEnabled(true)
	,m_Tag{tag}
{
}