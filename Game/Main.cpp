#include "MiniginPCH.h"

#include "Minigin.h"
#include "SceneManager.h"
#include "TransformComponent.h"
#include "RenderComponent.h"
#include "TextRenderComponent.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "InputCommands.h"
#include "Prefabs/PlayerPrefab.h"
#include "Box2DComponent.h"


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

int main(int, char*[]) {
	dae::Minigin engine;
	engine.Initialize();

	InputManager& input = ServiceLocator<InputManager, InputManager>::GetService();
	auto& sceneManager = ServiceLocator<dae::SceneManager, dae::SceneManager>::GetService();
	auto& resourceManager = ServiceLocator<dae::ResourceManager, dae::ResourceManager>::GetService();
	auto& gameTime = ServiceLocator<GameTime, GameTime>::GetService();

	input.MapCommand(ControllerButton::ButtonB, new QuitCommand());
	input.MapCommand(ControllerButton::ButtonY, new NextSceneCommand(), true);
	input.MapCommand(ControllerButton::ButtonX, new NextSceneCommand());

	// create all the scenes you need and add them to the scenemanager
	// Create Demo scene
	auto scene = sceneManager.CreateScene("Demo");

	// create object
	auto go = new GameObject();

	// create transform component
	TransformComponent* transformComponent = new TransformComponent{ go };

	//add transform component to object
	go->AddComponent(transformComponent);

	// create render component
	RenderComponent* renderComponent = new RenderComponent{ go, transformComponent };

	// add render component to object
	go->AddComponent(renderComponent);

	// set texture of render component
	renderComponent->SetTexture("../Data/background.jpg");

	// add object to scene
	scene->Add(go);

	// make next game object
	go = new GameObject();

	// create component and add it to object
	transformComponent = new TransformComponent{ go };
	go->AddComponent(transformComponent);

	// create component and add it to object
	renderComponent = new RenderComponent{ go, transformComponent };
	go->AddComponent(renderComponent);

	// Set variables of component
	renderComponent->SetTexture("../Data/logo.png");
	transformComponent->SetPosition(216, 180, 0);
	// add object to the scene
	scene->Add(go);

	auto font = resourceManager.LoadFont("../Data/Lingua.otf", 36);

	go = new GameObject();

	transformComponent = new TransformComponent{ go };
	go->AddComponent(transformComponent);

	TextRenderComponent* textRenderComponent = new TextRenderComponent{ go, transformComponent, 
		"Programming 4 Assignment", font };
	go->AddComponent(textRenderComponent);

	transformComponent->SetPosition(80, 20, 0);
	scene->Add(go);

	scene->Add(gameTime.GetRenderingObject());


	scene = sceneManager.CreateScene("Demo2");

	scene->Add(gameTime.GetRenderingObject());

	int width{};
	int height{};
	engine.GetWindowSize(width, height);
	scene = sceneManager.CreateScene("Level1", true, "Resources/level.dat", width, height);
	scene->Add(gameTime.GetRenderingObject());
	


	std::string sceneName = "Level1";
	sceneManager.SetActiveScene(sceneName);
	PlayerPrefab* playerObject = new PlayerPrefab();
	playerObject->Initialize(scene->GetPhysicsWorld());
	scene->Add(playerObject);

	// Actually run the game
	engine.Run();
    return 0;
}