#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

GameObject::~GameObject()
{
	for (int i{}; i < m_Components.size(); i++)
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
		m_Components[i]->Update();
	}
}

void GameObject::Render() const
{
	for (size_t i{}; i < m_Components.size(); i++)
	{
		m_Components[i]->Render();
	}
}

void GameObject::AddComponent(BaseComponent* pComponent)
{
	m_Components.push_back(pComponent);
}
