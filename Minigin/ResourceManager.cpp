#include "MiniginPCH.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Renderer.h"
#include "Texture2D.h"
#include "Font.h"

dae::ResourceManager::~ResourceManager()
{
	for (size_t i{}; i < m_Textures.size(); i++)
	{
		delete m_Textures[i];
		m_Textures[i] = nullptr;
	}
	m_Textures.clear();

	for (size_t i{}; i < m_Fonts.size(); i++)
	{
		delete m_Fonts[i];
		m_Fonts[i] = nullptr;
	}
	m_Fonts.clear();
}

void dae::ResourceManager::Init(const std::string& dataPath)
{
	m_DataPath = dataPath;

	// load support for png and jpg, this takes a while!

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) 
	{
		throw std::runtime_error(std::string("Failed to load support for png's: ") + SDL_GetError());
	}

	if ((IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG) != IMG_INIT_JPG) 
	{
		throw std::runtime_error(std::string("Failed to load support for jpg's: ") + SDL_GetError());
	}

	if (TTF_Init() != 0) 
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

dae::Texture2D* dae::ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = file;

	// don't load new resources if they already exist
	std::string fileName{};
	for (auto texture : m_Textures)
	{
		texture->GetFileName(fileName);
		if (fileName == file)
		{
			return texture;
		}
	}

	auto texture = IMG_LoadTexture(ServiceLocator<dae::Renderer, dae::Renderer>::GetService().GetSDLRenderer(), fullPath.c_str());
	if (texture == nullptr) 
	{
		throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
	}

	m_Textures.push_back(new Texture2D(texture, file));
	return m_Textures.back();
}

dae::Font* dae::ResourceManager::LoadFont(const std::string& file, unsigned int size)
{
	std::string fileName{};
	for (auto font : m_Fonts)
	{
		font->GetFileName(fileName);
		if (fileName == file)
		{
			return font;
		}
	}
	m_Fonts.push_back(new Font(file, size));
	return m_Fonts.back();
}
