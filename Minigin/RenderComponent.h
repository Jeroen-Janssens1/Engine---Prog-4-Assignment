#pragma once
#include "BaseComponent.h"
#include "Texture2D.h"
#include "MiniginPCH.h"

class TransformComponent;
class RenderComponent :
	public BaseComponent
{
public:
	RenderComponent(GameObject* pOwner, TransformComponent* pTransform, float width=-1.f, float height=-1.f, bool isSpriteSheet = false,
		int nrCols = 1, int nrRows = 1, int cellWidth = 0, int cellHeight = 0, int xPos = 0, int yPos = 0);
	virtual ~RenderComponent()=default;

	void Update() override;
	void Render() const override;

	float GetWidth() const { return m_Width; }
	float GetHeight() const { return m_Height; }

	void SetTexture(const std::string& filename);

protected:
	dae::Texture2D* m_Texture;
	TransformComponent* m_pTransformParent;
	int m_NrCols;
	int m_NrRows;
	int m_CellWidth;
	int m_CellHeight;
	int m_XPos;
	int m_YPos;
	bool m_IsSpriteSheet;
	float m_Width;
	float m_Height;
};

