#pragma once
#include "BaseComponent.h"
class RenderComponent;
class TransformComponent;

class TileComponent final :
	public BaseComponent
{

public:
	TileComponent(int xPos, int yPos, std::shared_ptr<RenderComponent> rendererReference, std::shared_ptr<TransformComponent> transformReference,
		std::shared_ptr<GameObject>& pOwner, const std::string& texturePath);
	~TileComponent() = default;

	void Update() override;
	void Render() const override;

	int GetXPos();
	int GetYPos();

private:
	std::weak_ptr<RenderComponent> m_RendererReference;
	std::weak_ptr<TransformComponent> m_TransformReference;
	int m_XPos;
	int m_YPos;
};

