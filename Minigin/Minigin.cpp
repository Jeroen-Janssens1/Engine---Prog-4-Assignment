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
#include "ServiceLocator.h"
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
	// init all services
	ServiceLocator<Renderer, Renderer>::Init();
	ServiceLocator<Renderer, Renderer>::GetService().Init(m_Window);
	ServiceLocator<InputManager, InputManager>::Init();
	ServiceLocator<ResourceManager, ResourceManager>::Init();
	ServiceLocator<SceneManager, SceneManager>::Init();
	ServiceLocator<ResourceManager, ResourceManager>::GetService().Init("../Data/");
	ServiceLocator<GameTime, GameTime>::Init();
	ServiceLocator<GameTime, GameTime>::GetService().Init();
	
	// Init the physics engine



	m_IsInitialized = true;
}

void dae::Minigin::Cleanup()
{
	
	ServiceLocator<Renderer, Renderer>::GetService().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run()
{
	if(!m_IsInitialized)
		Initialize();
	
	{
		auto& renderer = ServiceLocator<Renderer, Renderer>::GetService();
		auto& sceneManager = ServiceLocator<SceneManager, SceneManager>::GetService();
		auto& gameTime = ServiceLocator<GameTime, GameTime>::GetService();

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			
			doContinue = ServiceLocator<InputManager, InputManager>::GetService().ProcessInput();
			gameTime.Update();
			sceneManager.Update();
			renderer.Render();
			
			auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
void dae::Minigin::GetWindowSize(int& width, int& height)
{
	SDL_GetWindowSize(m_Window, &width, &height);
}
