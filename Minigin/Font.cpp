#include "MiniginPCH.h"
#include <SDL_ttf.h>
#include "Font.h"

TTF_Font* Font::GetFont() const {
	return m_pFont;
}

Font::Font(const std::string& fullPath, unsigned int size) : m_pFont(nullptr), m_Size(size), m_FileName(fullPath)
{
	m_pFont = TTF_OpenFont(fullPath.c_str(), size);
	if (m_pFont == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load font: ") + SDL_GetError());
	}
}

Font::~Font()
{
	TTF_CloseFont(m_pFont);
}

void Font::GetFileName(std::string& fileName) const
{
	fileName = m_FileName;
}

void Font::SetSize(unsigned int size)
{
	TTF_CloseFont(m_pFont);
	m_pFont = TTF_OpenFont(m_FileName.c_str(), size);
}
