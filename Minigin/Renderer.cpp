#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "ServiceLocator.h"


void Renderer::Init(SDL_Window * window)
{
	m_Renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void Renderer::Render() const
{
	SDL_RenderClear(m_Renderer);

	ServiceLocator<SceneManager, SceneManager>::GetService().Render();
	
	SDL_RenderPresent(m_Renderer);
}

void Renderer::Destroy()
{
	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, bool isFlipped) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	dst.x -= dst.w / 2;
	dst.y -= dst.h / 2;
	if (isFlipped)
	{
		SDL_Point center{};
		center.x = dst.x;
		center.y = dst.y;
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, 0, &center, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	}
	else
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, bool isFlipped) const
{
	SDL_Rect dst;
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(width);
	dst.h = static_cast<int>(height);
	dst.x -= dst.w / 2;
	dst.y -= dst.h / 2;
	if (isFlipped)
	{
		SDL_Point center{};
		center.x = dst.x;
		center.y = dst.y;
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst, 0, &center, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	}
	else
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, float x, float y, int xPos, int yPos, int srcWidth, int srcHeight, float width, float height, bool isFlipped) const
{
	SDL_Rect dst;
	SDL_Rect src;

	if (width < 0 || height < 0)
	{
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	}
	else
	{
		dst.x = static_cast<int>(x);
		dst.y = static_cast<int>(y);
		dst.w = static_cast<int>(width);
		dst.h = static_cast<int>(height);
	}
	dst.x -= dst.w / 2;
	dst.y -= dst.h / 2;
	src.x = xPos;
	src.y = yPos;
	src.w = srcWidth;
	src.h = srcHeight;
	if (isFlipped)
	{
		SDL_Point center{};
		center.x = dst.x;
		center.y = dst.y;
		SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst, 0, &center, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
	}
	else
		SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst);
}
