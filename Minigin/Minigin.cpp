#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "GameObject.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "TextRenderComponent.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "MiniginPCH.h"
#include "Minigin.h"
#include "SDL.h"
using namespace std;
using namespace std::chrono;

void dae::Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");
	// create the GameTime singleton and init it
	GameTime::GetInstance().Init();
	m_Input.reset();
	m_Input = std::make_shared<InputManager>();

	m_IsInitialized = true;
}

void dae::Minigin::Cleanup()
{
	
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	if(!m_IsInitialized)
		Initialize();
	
	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			
			doContinue = m_Input->ProcessInput();
			GameTime::GetInstance().Update();
			sceneManager.Update();
			renderer.Render();
			
			auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}

std::shared_ptr<InputManager> dae::Minigin::GetInputManager()
{
	return m_Input;
}

void dae::Minigin::GetWindowSize(int& width, int& height)
{
	SDL_GetWindowSize(m_Window, &width, &height);
}
