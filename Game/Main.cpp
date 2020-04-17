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


#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

int main(int, char*[]) {
	dae::Minigin engine;
	engine.Initialize();

	InputManager& input = ServiceLocator<InputManager, InputManager>::GetService();

	input.MapCommand<QuitCommand>(ControllerButton::ButtonB);
	input.MapCommand<NextSceneCommand>(ControllerButton::ButtonY, true);
	input.MapCommand<NextSceneCommand>(ControllerButton::ButtonX);

	// create all the scenes you need and add them to the scenemanager
	// Create Demo scene
	std::shared_ptr<BaseComponent> addComponent;
	auto scene = dae::SceneManager::GetInstance().CreateScene("Demo");

	// create object
	auto go = std::make_shared<GameObject>();

	// create transform component
	std::shared_ptr<TransformComponent> transformComponent = std::make_shared<TransformComponent>(TransformComponent{ go });

	//add transform component to object
	addComponent = std::static_pointer_cast<BaseComponent>(transformComponent);
	go->AddComponent(addComponent);

	// create render component
	std::shared_ptr<RenderComponent> renderComponent = std::make_shared<RenderComponent>(RenderComponent{ go, transformComponent });

	// add transform component to object
	addComponent = std::static_pointer_cast<BaseComponent>(renderComponent);
	go->AddComponent(addComponent);

	// set texture of render component
	renderComponent->SetTexture("../Data/background.jpg");

	// add object to scene
	scene->Add(go);

	// make next game object
	go = std::make_shared<GameObject>();

	// create component and add it to object
	transformComponent = std::make_shared<TransformComponent>(TransformComponent{ go });
	addComponent = std::static_pointer_cast<BaseComponent>(transformComponent);
	go->AddComponent(addComponent);

	// create component and add it to object
	renderComponent = std::make_shared<RenderComponent>(RenderComponent{ go, transformComponent });
	addComponent = std::static_pointer_cast<BaseComponent>(renderComponent);
	go->AddComponent(addComponent);

	// Set variables of component
	renderComponent->SetTexture("../Data/logo.png");
	transformComponent->SetPosition(216, 180, 0);
	// add object to the scene
	scene->Add(go);

	auto font = dae::ResourceManager::GetInstance().LoadFont("../Data/Lingua.otf", 36);

	go = std::make_shared<GameObject>();

	transformComponent = std::make_shared<TransformComponent>(TransformComponent{ go });
	addComponent = std::static_pointer_cast<BaseComponent>(transformComponent);
	go->AddComponent(addComponent);

	std::shared_ptr<TextRenderComponent> textRenderComponent = std::make_shared<TextRenderComponent>
		(TextRenderComponent{ go, transformComponent, "Programming 4 Assignment", font });
	addComponent = std::static_pointer_cast<BaseComponent>(textRenderComponent);
	go->AddComponent(addComponent);

	transformComponent->SetPosition(80, 20, 0);
	scene->Add(go);

	scene->Add(GameTime::GetInstance().GetRenderingObject());


	scene = dae::SceneManager::GetInstance().CreateScene("Demo2");

	scene->Add(GameTime::GetInstance().GetRenderingObject());

	int width{};
	int height{};
	engine.GetWindowSize(width, height);
	scene = dae::SceneManager::GetInstance().CreateScene("Level1", true, "Resources/level.dat", width, height);
	scene->Add(GameTime::GetInstance().GetRenderingObject());
	


	std::string sceneName = "Level1";
	dae::SceneManager::GetInstance().SetActiveScene(sceneName);
	std::shared_ptr<PlayerPrefab> playerObject = std::make_shared<PlayerPrefab>();
	playerObject->Initialize(playerObject);
	scene->Add(playerObject);

	// Actually run the game
	engine.Run();
    return 0;
}