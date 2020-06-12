#include "LevelBehaviour.h"
#include "Services.h"
#include "BinaryReader.h"
#include "Scene.h"
#include "PlayerBehaviour.h"
#include "ContactListener.h"

LevelBehaviour::LevelBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	,m_Timer{5.f}
{
}

LevelBehaviour::~LevelBehaviour()
{
	delete m_ContactListener;
}

void LevelBehaviour::Initialize(const std::string& levelName, const std::string& levelPath, const std::string& nextScene)
{
	m_NextScene = nextScene;
	// Load the level from the levelPath file using the binary reader from the engine
	auto binReader = BinaryReader();
	Scene* scene = nullptr;
	float p1XPos{};
	float p1YPos{};
	float p2XPos{};
	float p2YPos{};
	if (binReader.OpenFile(levelPath))
	{
		// The level file contains the position for player 1, the position for player 2 (for co-op and versus), nr of enemies
		// and for every enemy it holds the type in a string and their position
		// then it has a string that holds the path to the .tm file for creating the scene

		// for now, skip the player positions and the enemy nr since they aren't relevant yet
		int intDumpValue{};
		binReader.Read<float>(p1XPos);
		binReader.Read<float>(p1YPos);
		binReader.Read<float>(p2XPos);
		binReader.Read<float>(p2YPos);
		binReader.Read<int>(intDumpValue);
		
		std::string tmPath{};
		binReader.ReadString(tmPath);
		
		scene = SceneService.CreateScene(levelName, true, tmPath);
	}

	if (scene)
	{
		scene->Add(m_pOwner);
		// add the created enemies and players into the scene here once it becomes relevant
		// add player
		auto* go = new GameObject("Player");
		auto* playerBehaviour = new PlayerBehaviour(go);
		go->AddComponent(playerBehaviour);
		playerBehaviour->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 0, p1XPos, p1YPos);
		scene->Add(go);
		m_ContactListener = new ContactListener();
		scene->GetPhysicsWorld()->SetContactListener(m_ContactListener);
		go = GameTimeService.GetRenderingObject();
		auto* tc = go->GetComponent<TransformComponent>();
		tc->SetPosition(100.f, 450.f, 0.f);
		scene->Add(go);
	}
	
	
}

void LevelBehaviour::Update()
{
	auto& gameTime = GameTimeService;
	// check if there are any enemies left enabled/not death
	for (auto* enemy : m_Enemies)
	{
		if (enemy->GetIsEnabled())
		{
			return;
		}
	}

	// if no enemies left, update the timer
	m_Timer -= gameTime.GetElapsed();

	// if timer reached value, proceed to next scene
	if (m_Timer <= 0.f)
	{
		//SceneService.SetActiveScene(m_NextScene);
	}
}

void LevelBehaviour::Render() const
{
}

void LevelBehaviour::OnLoad()
{
	// set all enemies to active and to their correct locations
	// set player to correct location based on game mode
	m_Timer = 5.f;
}
