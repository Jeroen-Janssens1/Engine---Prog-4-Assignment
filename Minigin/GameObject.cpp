#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

GameObject::GameObject(const std::string& tag)
	:m_Tag{tag}
	,m_IsEnabled{true}
{
}

GameObject::~GameObject()
{
	for (size_t i{}; i < m_Components.size(); i++)
	{
		delete m_Components[i];
		m_Components[i] = nullptr;
	}
	m_Components.clear();
}

void GameObject::Update()
{
	for (size_t i{}; i < m_Components.size(); i++)
	{
		if(m_Components[i]->GetIsEnabled())
			m_Components[i]->Update();
	}
}

void GameObject::Render() const
{
	for (size_t i{}; i < m_Components.size(); i++)
	{
		if(m_Components[i]->GetIsEnabled())
			m_Components[i]->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	m_Components.push_back(pComponent);
}

void GameObject::OnLoad()
{
	for (size_t i{}; i < m_Components.size(); i++)
	{
		m_Components[i]->OnLoad();
	}
}