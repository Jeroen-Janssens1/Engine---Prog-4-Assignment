#pragma once
struct SDL_Window;
class PhysicsContactListener;
class PhysicsContactFilter;
class InputManager;

	class Minigin
	{
	public:
		Minigin() = default;
		~Minigin();
		void Initialize();
		void Cleanup();
		void Run();
		void GetWindowSize(int& width, int& height);

		Minigin(const Minigin&) = delete;
		Minigin(Minigin&&) = delete;
		Minigin& operator=(const Minigin&) = delete;
		Minigin& operator=(Minigin&&) = delete;
	private:
		SDL_Window* m_Window{};
		bool m_IsInitialized=false;
		PhysicsContactFilter* m_pContactFilter;
		PhysicsContactListener* m_pContactListener;
	};