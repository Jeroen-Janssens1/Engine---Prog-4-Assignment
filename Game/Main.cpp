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

	gameTime.SetFont("Resources/nintendo-nes-font.ttf");
	gameTime.SetFontSize(16);

	//input.MapCommand(0, ControllerButton::ButtonB, new QuitCommand());
	//input.MapCommand(0, ControllerButton::ButtonY, new NextSceneCommand(), true);
	//input.MapCommand(0, ControllerButton::ButtonX, new NextSceneCommand());

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

	scene = sceneManager.CreateScene("Level1", true, "Resources/level1.tm");
	scene = sceneManager.CreateScene("Level1", true, "Resources/level2.tm");
	scene = sceneManager.CreateScene("Level1", true, "Resources/level3.tm");
	input.MapCommand(VK_RIGHT, new NextSceneCommand(), true);


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