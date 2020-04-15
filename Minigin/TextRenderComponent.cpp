#include "MiniginPCH.h"
#include "TextRenderComponent.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "TransformComponent.h"


TextRenderComponent::TextRenderComponent(std::shared_ptr<GameObject>& pOwner, std::shared_ptr<TransformComponent>& pTransform, const std::string& text, const std::shared_ptr<dae::Font>& font)
	:RenderComponent{pOwner, pTransform}
	,m_Text{text}
	,m_Font{font}
	,m_NeedsUpdate{true}
{
	
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
		auto texture = SDL_CreateTextureFromSurface(dae::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_Texture = std::make_shared<dae::Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void TextRenderComponent::Render() const
{
	if (m_Texture != nullptr)
	{
		const auto pos = m_pTransformParent.lock()->GetPosition();
		dae::Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
	}
}

void TextRenderComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}
