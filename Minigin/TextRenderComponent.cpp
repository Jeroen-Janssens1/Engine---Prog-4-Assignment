#include "MiniginPCH.h"
#include "TextRenderComponent.h"
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "Texture2D.h"
TextRenderComponent::TextRenderComponent(GameObject* pOwner, TransformComponent* pTransform, const std::string& text, Font* font,
	SDL_Color color, const std::string& tag)
	:BaseComponent{pOwner, tag}
	,m_Text{text}
	,m_pFont{font}
	,m_NeedsUpdate{true}
	,m_pTransformParent{pTransform}
	,m_Color{color}
{
}

TextRenderComponent::~TextRenderComponent()
{
	delete m_pTexture;
}

void TextRenderComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_pFont->GetFont() , m_Text.c_str(), m_Color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(ServiceLocator<Renderer, Renderer>::GetService().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		if (m_pTexture)
		{
			delete m_pTexture;
			m_pTexture = nullptr;
		}
		m_pTexture = new Texture2D(texture, "");
		m_NeedsUpdate = false;
	}
}

void TextRenderComponent::Render() const
{
	if (m_pTexture != nullptr)
	{
		const auto pos = m_pTransformParent->GetPosition();
		ServiceLocator<Renderer, Renderer>::GetService().RenderTexture(*m_pTexture, pos.x, pos.y);
	}
}

void TextRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
