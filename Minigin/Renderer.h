#pragma once

struct SDL_Window;
struct SDL_Renderer;

	class Texture2D;
	/**
	 * Simple RAII wrapper for the SDL renderer
	 */
	class Renderer final
	{
	public:
		Renderer() = default;
		~Renderer() = default;
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void RenderTexture(const Texture2D& texture, float x, float y, bool isFlipped = false) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height, bool isFlipped = false) const;
		void RenderTexture(const Texture2D& texture, float x, float y, int xPos, int yPos, int srcWidth, int srcHeight, float width = -1.f, float height = -1.f, bool isFlipped = false) const;

		SDL_Renderer* GetSDLRenderer() const { return m_Renderer; }
	private:
		SDL_Renderer* m_Renderer{};
	};
