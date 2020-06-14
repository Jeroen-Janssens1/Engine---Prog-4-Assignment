#pragma once
#include "BaseComponent.h"
class RenderComponent;
class TransformComponent;

class TileComponent final :
	public BaseComponent
{
public:
	TileComponent(int xPos, int yPos,
		GameObject* pOwner);
	virtual ~TileComponent() = default;

	void Update() override {};
	void Render() const override {};
	void OnLoad() override {}

	int GetXPos();
	int GetYPos();

private:
	int m_XPos;
	int m_YPos;
};

