#pragma once
#include "BaseComponent.h"
#include <string>
#include "MiniginPCH.h"
#include "Font.h"
#include <SDL.h>

class TransformComponent;
class Texture2D;
class TextRenderComponent final:
	public BaseComponent
{
public:
	TextRenderComponent(GameObject* pOwner, TransformComponent* pTransform, const std::string& text, Font* font,
		SDL_Color color = SDL_Color{255, 255, 255}, const std::string& tag = "");
	virtual ~TextRenderComponent();

	void Update() override;
	void Render() const override;
	void OnLoad() override {}

	void SetText(const std::string& text);

	void SetFont(Font* font) { m_Font = font; m_NeedsUpdate = true; }

	void SetFontSize(unsigned int size) { m_Font->SetSize(size); m_NeedsUpdate = true; }

	void SetColor(SDL_Color color) { m_Color = color; m_NeedsUpdate = true; }
	

protected:
	bool m_NeedsUpdate;
	std::string m_Text;
	Font* m_Font;

	Texture2D* m_Texture;
	TransformComponent* m_pTransformParent;

	SDL_Color m_Color;

};