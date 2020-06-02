#pragma once
#include "BaseComponent.h"
class RenderComponent;
class TransformComponent;

class TileComponent final :
	public BaseComponent
{

public:
	TileComponent(int xPos, int yPos, RenderComponent* rendererReference, TransformComponent* transformReference,
		GameObject* pOwner, const std::string& texturePath);
	virtual ~TileComponent() = default;

	void Update() override;
	void Render() const override;

	int GetXPos();
	int GetYPos();

private:
	RenderComponent* m_RendererReference;
	TransformComponent* m_TransformReference;
	int m_XPos;
	int m_YPos;
};

