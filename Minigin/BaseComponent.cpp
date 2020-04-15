#include "MiniginPCH.h"
#include "BaseComponent.h"

BaseComponent::BaseComponent(std::shared_ptr<GameObject>& pOwner)
	:m_pOwner{pOwner}
{
	
}