#include "MiniginPCH.h"
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

GameObject::~GameObject()
{
	m_Components.clear();
}

void GameObject::Update()
{
	for (int i{}; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}

void GameObject::Render() const
{
	for (int i{}; i < m_Components.size(); i++)
	{
		m_Components[i]->Render();
	}
}

void GameObject::AddComponent(std::shared_ptr<BaseComponent>& component)
{
	m_Components.push_back(component);
}
