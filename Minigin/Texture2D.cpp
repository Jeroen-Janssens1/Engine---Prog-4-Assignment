#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>

Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_pTexture);
}

void Texture2D::GetFileName(std::string& fileName) const
{
	fileName = m_FileName;
}

SDL_Texture* Texture2D::GetSDLTexture() const
{
	return m_pTexture;
}

Texture2D::Texture2D(SDL_Texture* texture, const std::string& fileName)
{
	m_pTexture = texture;
	m_FileName = fileName;
}
