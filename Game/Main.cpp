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

	// Main Menu Scene
	auto* scene = sceneManager.CreateScene("MainMenu");

	auto* go = new GameObject();
	auto* mainMenu = new MainMenu(go, scene);
	go->AddComponent(mainMenu);
	mainMenu->Initialize(width, height, font);
	scene->Add(go);

	sceneManager.SetActiveScene(0);

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

	// Test Environment
	//scene = sceneManager.CreateScene("Level1", true, "Resources/level.dat");
	//scene->Add(gameTime.GetRenderingObject());
	//


	//std::string sceneName = "Level1";
	//sceneManager.SetActiveScene(sceneName);
	//go = new GameObject();
	//auto* playerBehaviour = new PlayerBehaviour(go);
	//go->AddComponent(playerBehaviour);
	//playerBehaviour->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 0);
	//scene->Add(go);
	//// second player
	//go = new GameObject();
	//playerBehaviour = new PlayerBehaviour(go);
	//go->AddComponent(playerBehaviour);
	//playerBehaviour->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 1);
	//scene->Add(go);

	// Actually run the game
	engine.Run();
    return 0;
}