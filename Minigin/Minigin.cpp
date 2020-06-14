#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "SoundManager.h"
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
#include "PhysicsVariables.h"
#include "SDL_ttf.h"
#include <future>

using namespace std;
using namespace std::chrono;

Minigin::~Minigin()
{
	delete m_pContactFilter;
	delete m_pContactListener;
}

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) 
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
	// init sdl audio
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		throw std::runtime_error(std::string("Core::Initialize( ), error when calling Mix_OpenAudio!: ") + Mix_GetError());
	}
	m_pContactFilter = new PhysicsContactFilter();
	m_pContactListener = new PhysicsContactListener();

	// init all services
	ServiceLocator<Renderer, Renderer>::Init();
	ServiceLocator<Renderer, Renderer>::GetService().Init(m_Window);
	ServiceLocator<InputManager, InputManager>::Init();
	ServiceLocator<ResourceManager, ResourceManager>::Init();
	ServiceLocator<SceneManager, SceneManager>::Init();
	ServiceLocator<GameTime, GameTime>::Init();
	ServiceLocator<SoundManager, SoundManager>::Init();
	ServiceLocator<PhysicsVariables, PhysicsVariables>::Init();
	ServiceLocator<PhysicsVariables, PhysicsVariables>::GetService().SetContactListener(m_pContactListener);
	ServiceLocator<PhysicsVariables, PhysicsVariables>::GetService().SetContactFilter(m_pContactFilter);
	ServiceLocator<ResourceManager, ResourceManager>::GetService().Init("../Data/");
	ServiceLocator<GameTime, GameTime>::GetService().Init();

	m_IsInitialized = true;
}

void Minigin::Cleanup()
{
	ServiceLocator<Renderer, Renderer>::GetService().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Minigin::Run()
{
	if(!m_IsInitialized)
		Initialize();

	auto& renderer = ServiceLocator<Renderer, Renderer>::GetService();
	auto& sceneManager = ServiceLocator<SceneManager, SceneManager>::GetService();
	auto& gameTime = ServiceLocator<GameTime, GameTime>::GetService();

	bool doContinue = true;
	while (doContinue)
	{	
		gameTime.Update(); // update deltaTime and FPS counter
		doContinue = sceneManager.ProcessInput();
		// only check global input if scene specific input didn't request to stop playing
		if (doContinue)
			doContinue = ServiceLocator<InputManager, InputManager>::GetService().ProcessInput();
		sceneManager.Update();
		// run the physics update for the next frame in a threat and simultaniously run the renderer to visualize the current frame
		std::thread t(&SceneManager::PhysicsUpdate, std::ref(sceneManager));
		renderer.Render();
		// make sure the physics are done updating before continuing to the next frame
		t.join();
	}
	Cleanup();
}
void Minigin::GetWindowSize(int& width, int& height)
{
	SDL_GetWindowSize(m_Window, &width, &height);
}
