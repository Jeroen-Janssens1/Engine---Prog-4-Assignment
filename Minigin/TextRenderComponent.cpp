#include "MiniginPCH.h"
#include "TextRenderComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"


TextRenderComponent::TextRenderComponent(GameObject* pOwner, TransformComponent* pTransform, const std::string& text, dae::Font* font)
	:RenderComponent{pOwner, pTransform}
	,m_Text{text}
	,m_Font{font}
	,m_NeedsUpdate{true}
{
	
}

TextRenderComponent::~TextRenderComponent()
{
	delete m_Texture;
}

void TextRenderComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const SDL_Color color = { 255, 255, 255 }; // only white text for now
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont() , m_Text.c_str(), color);
		if (surf == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(ServiceLocator<dae::Renderer, dae::Renderer>::GetService().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		if (m_Texture != nullptr)
		{
			delete m_Texture;
			m_Texture = nullptr;
		}
		if (m_Texture)
		{
			delete m_Texture;
			m_Texture = nullptr;
		}
		m_Texture = new dae::Texture2D(texture, "");
		m_NeedsUpdate = false;
	}
}

void TextRenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto pos = m_pTransformParent->GetPosition();
		ServiceLocator<dae::Renderer, dae::Renderer>::GetService().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void TextRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
