#pragma once
#include "BaseComponent.h"
#include "MiniginPCH.h"

class TransformComponent;
class Texture2D;
class RenderComponent final :
	public BaseComponent
{
public:
	RenderComponent(GameObject* pOwner, TransformComponent* pTransform, const std::string& tag = "",
		float width=-1.f, float height=-1.f, bool isSpriteSheet = false,
		int cellWidth = 0, int cellHeight = 0, int xPos = 0, int yPos = 0);
	virtual ~RenderComponent()=default;

	void Update() override;
	void Render() const override;
	void OnLoad() override {}

	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	int GetCellWidth() const { return m_CellWidth; }
	int GetCellHeight() const { return m_CellHeight; }

	void SetTexture(const std::string& filename);
	void SetSpritePos(int xPos, int yPos);

	void SetIsFlipped(bool value) { m_IsFlipped = value; }

protected:
	Texture2D* m_Texture;
	TransformComponent* m_pTransformParent;
	int m_CellWidth;
	int m_CellHeight;
	int m_XPos;
	int m_YPos;
	bool m_IsSpriteSheet;
	float m_Width;
	float m_Height;
	bool m_IsFlipped;
};

