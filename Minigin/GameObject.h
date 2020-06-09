#pragma once
#include "Components.h"
#include <vector>
#include <string>
	class Texture2D;
	class GameObject final
	{
	public:
		virtual void Update();
		virtual void Render() const;

		void AddComponent(BaseComponent* pComponent);
		
		// This function is extremely slow! Do not use it in update and drawing/rendering functions!
		template<typename T>
		T* GetComponent() const
		{
			for (size_t i{}; i < m_Components.size(); i++)
			{
				if (dynamic_cast<T*>(m_Components[i]))
					return (T*)m_Components[i];
			}
			return nullptr;
		}

		GameObject(const std::string& tag = "");
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<BaseComponent*> m_Components;
		std::string m_Tag;
	};
