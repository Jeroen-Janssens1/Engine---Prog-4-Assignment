#include "LevelBehaviour.h"
#include "Services.h"
#include "BinaryReader.h"
#include "Scene.h"
#include "PlayerBehaviour.h"
#include "GameObject.h"
#include "Box2D.h"
#include "ZenBehaviour.h"

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
		int nrEnemies{};
		binReader.Read<float>(p1XPos);
		binReader.Read<float>(p1YPos);
		binReader.Read<float>(p2XPos);
		binReader.Read<float>(p2YPos);

		std::string tmPath{};
		binReader.ReadString(tmPath);
		scene = SceneService.CreateScene(levelName, true, tmPath);

		if (scene)
		{
			scene->Add(m_pOwner);
			// add the created enemies and players into the scene here once it becomes relevant
			// add player
			auto* go = new GameObject("Player");
			m_Player = new PlayerBehaviour(go);
			go->AddComponent(m_Player);
			m_Player->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 0, p1XPos, p1YPos);
			scene->Add(go);
			go = GameTimeService.GetRenderingObject();
			auto* tc = go->GetComponent<TransformComponent>();
			tc->SetPosition(100.f, 450.f, 0.f);
			scene->Add(go);


			// Load in the enemies
			binReader.Read<int>(nrEnemies);

			for (int i{}; i < nrEnemies; i++)
			{
				float xPos{};
				float yPos{};
				std::string enemyType{};
				binReader.ReadString(enemyType);
				binReader.Read<float>(xPos);
				binReader.Read<float>(yPos);
				if (enemyType == "zen")
					CreateZen(xPos, yPos, scene->GetPhysicsWorld(), scene);
			}
		}
	}

	// set the HUD up
	auto* font = ResourceService.LoadFont("Resources/nintendo-nes-font.ttf", 8);
	float yPos = 8;
	float xPos{};
	m_UIRenders.resize(4);
	for (int i{}; i < m_UIRenders.size(); i++)
	{
		auto* go = new GameObject();
		auto* tc = new TransformComponent(go);
		auto* trc = new TextRenderComponent(go, tc, "0", font);
		go->AddComponent(tc);
		go->AddComponent(trc);
		scene->Add(go);

		// position it correctly
		if (i == 0)
			xPos = 32;
		else if (i == 1)
			xPos = 80;
		else if (i == 2)
			xPos = 128;
		else if (i == 3)
			xPos = 176;
		tc->SetPosition(xPos, yPos, 0.f);

		m_UIRenders[i] = trc;
	}
	m_PlayerLives = 0;

}

void LevelBehaviour::Update()
{
	// update HUD if necessary
	auto newLives = m_Player->GetLives();
	if (newLives != m_PlayerLives)
	{
		m_PlayerLives = newLives;
		std::string lives{ std::to_string(m_PlayerLives) };
		m_UIRenders[int(UIIcon::P1Life)]->SetText(lives);
	}
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
		SceneService.SetActiveScene(m_NextScene);
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

void LevelBehaviour::CreateZen(float xPos, float yPos, b2World* physicsWorld, Scene* scene)
{
	auto* go = new GameObject("Enemy");
	auto* zenBehaviour = new ZenBehaviour(go);
	zenBehaviour->Initialize(physicsWorld, xPos, yPos);
	go->AddComponent(zenBehaviour);
	scene->Add(go);
	m_Enemies.push_back(go);
	
}
