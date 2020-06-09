#pragma once
#include "RenderComponent.h"
#include <string>
#include "MiniginPCH.h"
#include "Font.h"

class TextRenderComponent final:
	public RenderComponent
{
public:
	TextRenderComponent(GameObject* pOwner, TransformComponent* pTransform, const std::string& text, Font* font);
	virtual ~TextRenderComponent();

	void Update() override;
	void Render() const override;

	void SetText(const std::string& text);
	

protected:
	bool m_NeedsUpdate;
	std::string m_Text;
	Font* m_Font;

};