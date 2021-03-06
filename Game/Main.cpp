#include "MiniginPCH.h"

#include "Minigin.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextRenderComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameTime.h"
#include "InputCommands.h"
#include "Box2DComponent.h"
#include "Services.h"
#include "PlayerBehaviour.h"
#include "LevelBehaviour.h"


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

int main(int, char*[]) {
	Minigin engine;
	engine.Initialize();

	// Get all services for ease of use
	InputManager& input = InputService;
	auto& sceneManager = SceneService;
	auto& resourceManager = ResourceService;
	auto& gameTime = GameTimeService;
	auto& soundManager = SoundService;
	auto& physicVars = PhysicsVariablesService;
	
	physicVars.SetPixelsPerUnit(32);

	gameTime.SetFont("Resources/nintendo-nes-font.ttf");
	gameTime.SetFontSize(16);

	auto* font = resourceManager.LoadFont("Resources/nintendo-nes-font.ttf", 8);
	int width{};
	int height{};
	engine.GetWindowSize(width, height);

	// set the sound up
	soundManager.AddSound(0, "Resources/Audio/Theme.mp3", "Theme", true);
	soundManager.AddSound(1, "Resources/Audio/Jump.wav", "Jump", false);
	soundManager.AddSound(2, "Resources/Audio/Fire.wav", "Fire", false);
	soundManager.SetAllVolume(50);
	soundManager.SetSoundVolume("Theme", 25);

	// Main Menu Scene
	auto* scene = sceneManager.CreateScene("MainMenu");

	// Game Object that will manage the main menu functionality through a 'script' (class that inherits from BaseComponent)
	auto* go = new GameObject();
	auto* mainMenu = new MainMenu(go, scene);
	go->AddComponent(mainMenu);
	mainMenu->Initialize(width, height, font);
	scene->Add(go);
	
	sceneManager.SetActiveScene(0);

	// Load in the 3 levels
	go = new GameObject();
	auto* level = new LevelBehaviour(go);
	go->AddComponent(level);
	level->Initialize("level1", "Resources/levels/level1.bbl", "level2");
	
	go = new GameObject();
	level = new LevelBehaviour(go);
	go->AddComponent(level);
	level->Initialize("level2", "Resources/levels/level2.bbl", "level3");

	go = new GameObject();
	level = new LevelBehaviour(go);
	go->AddComponent(level);
	level->Initialize("level3", "Resources/levels/level3.bbl", "MainMenu");

	// Actually run the game
	engine.Run();
    return 0;
}