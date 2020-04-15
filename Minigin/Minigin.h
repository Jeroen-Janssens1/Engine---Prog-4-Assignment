#pragma once
struct SDL_Window;
class InputManager;
namespace dae
{
	class Minigin
	{
	public:
		void Initialize();
		//void LoadGame() const;
		void Cleanup();
		void Run();
		std::shared_ptr<InputManager> GetInputManager();
	private:
		static const int MsPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* m_Window{};
		std::shared_ptr<InputManager> m_Input{};
		bool m_IsInitialized=false;
	};
}