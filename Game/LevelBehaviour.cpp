#include "LevelBehaviour.h"
#include "Services.h"
#include "BinaryReader.h"
#include "Scene.h"
#include "PlayerBehaviour.h"
#include "GameObject.h"
#include "Box2D.h"
#include "ZenBehaviour.h"
#include "TransformComponent.h"

LevelBehaviour::GameType LevelBehaviour::m_GameMode = LevelBehaviour::GameType::SinglePlayer;

unsigned int LevelBehaviour::m_Player1Lives = 0;
unsigned int LevelBehaviour::m_Player2Lives = 0;
unsigned int LevelBehaviour::m_Player1Score = 0;
unsigned int LevelBehaviour::m_Player2Score = 0;

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
	m_PlayerTransform = nullptr;
	m_Player2Transform = nullptr;
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
			m_PlayerTransform = go->GetComponent<TransformComponent>();

			// add player
			go = new GameObject("Player");
			m_Player2 = new PlayerBehaviour(go);
			go->AddComponent(m_Player2);
			m_Player2->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 1, p2XPos, p2YPos);
			scene->Add(go);
			m_Player2Transform = go->GetComponent<TransformComponent>();

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
	m_Player1Lives = 0;
	m_Player2Lives = 0;
	m_Player1Score = 0;
	m_Player2Score = 0;

}

void LevelBehaviour::Update()
{
	if (m_Player1Lives == 0)
		m_Player->Kill();
	if (m_Player2Lives == 0)
		m_Player2->Kill();

	if(m_Player1Lives == 0 && m_Player2Lives == 0)
		SceneService.SetActiveScene(0);

	// update HUD if necessary
	auto newLives = m_Player->GetLives();
	if (newLives != m_Player1Lives)
	{
		m_Player1Lives = newLives;
		std::string lives{ std::to_string(m_Player1Lives) };
		m_UIRenders[int(UIIcon::P1Life)]->SetText(lives);
	}
	auto newScore = m_Player->GetScore();
	if (newScore != m_Player1Score)
	{
		m_Player1Score = newScore;
		std::string score{ std::to_string(m_Player1Score) };
		m_UIRenders[int(UIIcon::P1Score)]->SetText(score);
	}
	newLives = m_Player2->GetLives();
	if (newLives != m_Player2Lives)
	{
		m_Player2Lives = newLives;
		std::string lives{ std::to_string(m_Player2Lives) };
		m_UIRenders[int(UIIcon::P2Life)]->SetText(lives);
	}
	newScore = m_Player2->GetScore();
	if (newScore != m_Player2Score)
	{
		m_Player2Score = newScore;
		std::string score{ std::to_string(m_Player2Score) };
		m_UIRenders[int(UIIcon::P2Score)]->SetText(score);
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
	// set player lives and score to correct values from previous level
	m_Player->Reset();
	m_Player2->Reset();
	m_Player->SetLives(m_Player1Lives);
	m_Player->SetScore(m_Player1Score);
	m_Player2->SetLives(m_Player2Lives);
	m_Player2->SetScore(m_Player2Score);
	m_UIRenders[int(UIIcon::P1Life)]->SetText(std::to_string(m_Player1Lives));
	m_UIRenders[int(UIIcon::P2Life)]->SetText(std::to_string(m_Player2Lives));
	m_UIRenders[int(UIIcon::P1Score)]->SetText(std::to_string(m_Player1Score));
	m_UIRenders[int(UIIcon::P2Score)]->SetText(std::to_string(m_Player2Score));

	if (m_GameMode == GameType::SinglePlayer)
	{
		// disable player 2
		m_Player2->GetGameObject()->SetIsEnabled(false);
		m_Player2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(false);
		m_Player2Lives = 0;
		m_Player2->SetLives(m_Player2Lives);

		for (int i{}; i < m_Enemies.size(); i++)
		{
			ZenBehaviour* enemy = m_Enemies[i]->GetComponent<ZenBehaviour>();
			enemy->Reset();
			enemy->Set2PlayerMode(false);
		}
	}
	else if(m_GameMode == GameType::Coop)
	{
		// enable player 2 and set is Maita to false
		m_Player2->GetGameObject()->SetIsEnabled(true);
		m_Player2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(true);
		for (int i{}; i < m_Enemies.size(); i++)
		{
			ZenBehaviour* enemy = m_Enemies[i]->GetComponent<ZenBehaviour>();
			enemy->Reset();
			enemy->Set2PlayerMode(true);
		}
	}
	else if (m_GameMode == GameType::Versus)
	{
		// enable player 2 and set IsMaita variable to true
		m_Player2->GetGameObject()->SetIsEnabled(true);
		m_Player2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(true);
		for (int i{}; i < m_Enemies.size(); i++)
		{
			// Disable all enemies in versus mode
			m_Enemies[i]->GetComponent<ZenBehaviour>()->Kill();
		}
	}
}

void LevelBehaviour::CreateZen(float xPos, float yPos, b2World* physicsWorld, Scene* scene)
{
	auto* go = new GameObject("Enemy");
	auto* zenBehaviour = new ZenBehaviour(go);
	zenBehaviour->Initialize(physicsWorld, xPos, yPos, m_PlayerTransform, m_Player2Transform);
	go->AddComponent(zenBehaviour);
	scene->Add(go);
	m_Enemies.push_back(go);
	
}
