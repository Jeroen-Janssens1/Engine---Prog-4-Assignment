#pragma once
#include "BaseComponent.h"
#include <vector>
	class Texture2D;
	class GameObject
	{
	public:
		virtual void Update();
		virtual void Render() const;

		void AddComponent(std::shared_ptr<BaseComponent>& component);

		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
	};
