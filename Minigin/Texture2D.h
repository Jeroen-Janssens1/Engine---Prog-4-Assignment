#pragma once
#include <string>
struct SDL_Texture;
	/**
	 * Simple RAII wrapper for an SDL_Texture
	 */
	class Texture2D
	{
	public:
		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture, const std::string& fileName);
		~Texture2D();

		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;

		void GetFileName(std::string& fileName) const;
	private:
		SDL_Texture* m_pTexture;
		std::string m_FileName;
	};
