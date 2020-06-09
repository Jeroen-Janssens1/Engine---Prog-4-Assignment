#pragma once
struct SDL_Window;
class InputManager;

	class Minigin
	{
	public:
		void Initialize();
		//void LoadGame() const;
		void Cleanup();
		void Run();
		void GetWindowSize(int& width, int& height);
	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
		bool m_IsInitialized=false;
	};