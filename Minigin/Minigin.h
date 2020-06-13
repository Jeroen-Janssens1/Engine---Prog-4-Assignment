#pragma once
struct SDL_Window;
class PhysicsContactListener;
class PhysicsContactFilter;
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
		SDL_Window* m_Window{};
		bool m_IsInitialized=false;
		PhysicsContactFilter* m_pContactFilter;
		PhysicsContactListener* m_pContactListener;
	};