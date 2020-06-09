#pragma once

	class Texture2D;
	class Font;
	class ResourceManager final
	{
	public:
		ResourceManager() = default;
		~ResourceManager();

		ResourceManager(const ResourceManager& other) = delete;
		ResourceManager(ResourceManager&& other) = delete;
		ResourceManager& operator=(const ResourceManager& other) = delete;
		ResourceManager& operator=(ResourceManager&& other) = delete;

		void Init(const std::string& data);
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, unsigned int size);


	private:
		std::string m_DataPath;

		std::vector<Texture2D*> m_Textures;
		std::vector<Font*> m_Fonts;
	};