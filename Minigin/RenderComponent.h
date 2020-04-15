#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include "MiniginPCH.h"

class TransformComponent;
class RenderComponent :
	public BaseComponent
{
public:
	RenderComponent(std::shared_ptr<GameObject>& pOwner, std::shared_ptr<TransformComponent>& pTransform);
	~RenderComponent()=default;

	void Update() override;
	void Render() const override;

	void SetTexture(const std::string& filename);

protected:
	std::shared_ptr<dae::Texture2D> m_Texture;
	std::weak_ptr<TransformComponent> m_pTransformParent;
};

