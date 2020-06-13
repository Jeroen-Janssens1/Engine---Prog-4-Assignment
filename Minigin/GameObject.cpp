#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Scene.h"

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

// use this function if you want to delete an object from a scene, never use delete yourself!
void GameObject::Destroy()
{
	m_ParentScene->Remove(this);
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