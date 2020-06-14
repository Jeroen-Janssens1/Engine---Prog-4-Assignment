#pragma once
#include "Components.h"
#include <vector>
#include <string>
class Scene;
class Texture2D;
class GameObject final
{
public:
	GameObject(const std::string& tag = "");
	virtual ~GameObject();

	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	virtual void Update();
	virtual void Render() const;
	void OnLoad();

	void AddComponent(BaseComponent* pComponent);
	const std::string& GetTag() const { return m_Tag; }
	bool GetIsEnabled() const { return m_IsEnabled; }
	void SetIsEnabled(bool value) { m_IsEnabled = value; }
	void SetIndx(int value) { m_Indx = value; }
	void SetParentScene(Scene* value) { m_pParentScene = value; }
	void SetTag(const std::string& value) { m_Tag = value; }
		
	// This function is  slow because of the dynamic casting! Do not use it in update and drawing/rendering functions unless absolutely necessary!
	template<typename T>
	T* GetComponent(const std::string& tag = "") const
	{
		for (size_t i{}; i < m_Components.size(); i++)
		{
			if (tag.empty() || tag == m_Components[i]->GetTag())
				if (dynamic_cast<T*>(m_Components[i]))
					return (T*)m_Components[i];
		}
		return nullptr;
	}
	void Destroy();

private:
	Scene* m_pParentScene;
	std::vector<BaseComponent*> m_Components;
	std::string m_Tag;
	bool m_IsEnabled;
	int m_Indx;
};
