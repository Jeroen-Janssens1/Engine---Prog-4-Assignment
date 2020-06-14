#include "LevelBehaviour.h"
#include "Services.h"
#include "BinaryReader.h"
#include "Scene.h"
#include "PlayerBehaviour.h"
#include "GameObject.h"
#include "Box2D.h"
#include "ZenBehaviour.h"
#include "TransformComponent.h"
#include "MaitaBehaviour.h"
#include "UIObserver.h"

LevelBehaviour::GameType LevelBehaviour::m_GameMode = LevelBehaviour::GameType::SinglePlayer;

unsigned int LevelBehaviour::m_Player1Lives = 0;
unsigned int LevelBehaviour::m_Player2Lives = 0;
unsigned int LevelBehaviour::m_Player1Score = 0;
unsigned int LevelBehaviour::m_Player2Score = 0;

LevelBehaviour::LevelBehaviour(GameObject* pOwner)
	:BaseComponent(pOwner)
	,m_Timer{5.f}
	,m_TopY{16.f}
	,m_BottomY{416.f}
{
}

void LevelBehaviour::Initialize(const std::string& levelName, const std::string& levelPath, const std::string& nextScene)
{
	m_pPlayerTransform = nullptr;
	m_pPlayer2Transform = nullptr;
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
		// The level file contains the position for player 1, the position for player 2 (for co-op and versus) 
		//// then it has a string that holds the path to the .tm file for creating the scene and nr of enemies
		// and for every enemy it holds the type in a string and their position
		int nrEnemies{};
		binReader.Read<float>(p1XPos);
		binReader.Read<float>(p1YPos);
		binReader.Read<float>(p2XPos);
		binReader.Read<float>(p2YPos);

		std::string tmPath{};
		binReader.ReadString(tmPath);
		// create a level using the path to the .tm file
		scene = SceneService.CreateScene(levelName, true, tmPath);

		if (scene)
		{
			scene->Add(m_pOwner);
			// add player
			auto* go = new GameObject("Player");
			m_pPlayer = new PlayerBehaviour(go);
			go->AddComponent(m_pPlayer);
			m_pPlayer->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 0, p1XPos, p1YPos);
			scene->Add(go);
			m_pPlayerTransform = go->GetComponent<TransformComponent>();

			// add player 2
			go = new GameObject("Player");
			m_pPlayer2 = new PlayerBehaviour(go);
			go->AddComponent(m_pPlayer2);
			m_pPlayer2->Initialize(scene->GetInputManager(), scene->GetPhysicsWorld(), 1, p2XPos, p2YPos);
			scene->Add(go);
			m_pPlayer2Transform = go->GetComponent<TransformComponent>();

			// add FPS counter
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
				if (enemyType == "maita")
					CreateMaita(xPos, yPos, scene->GetPhysicsWorld(), scene);
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
	// reset static variables
	m_Player1Lives = 0;
	m_Player2Lives = 0;
	m_Player1Score = 0;
	m_Player2Score = 0;

	// create the observers for the UI that will be observing the player characters
	auto* observer = new UIObserver(m_pOwner, m_UIRenders[int(UIIcon::P1Life)], m_UIRenders[int(UIIcon::P1Score)]);
	m_pPlayer->GetSubject()->AddObserver(observer);
	m_pOwner->AddComponent(observer);
	observer = new UIObserver(m_pOwner, m_UIRenders[int(UIIcon::P2Life)], m_UIRenders[int(UIIcon::P2Score)]);
	m_pPlayer2->GetSubject()->AddObserver(observer);
	m_pOwner->AddComponent(observer);
}

void LevelBehaviour::Update()
{
	// kill players if they're supposed to be death
	if (m_Player1Lives == 0)
		m_pPlayer->Kill();
	if (m_Player2Lives == 0)
		m_pPlayer2->Kill();
	
	// keep everything in the boundaries of the level
	if (m_pPlayerTransform->GetPosition().y > m_BottomY)
		m_pPlayerTransform->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(m_pPlayerTransform->GetPosition().x, m_TopY);
	if (m_pPlayerTransform->GetPosition().y < m_TopY)
		m_pPlayerTransform->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(m_pPlayerTransform->GetPosition().x, m_BottomY);
	if (m_pPlayer2Transform->GetPosition().y > m_BottomY)
		m_pPlayer2Transform->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(m_pPlayer2Transform->GetPosition().x, m_TopY);
	if (m_pPlayer2Transform->GetPosition().y < m_TopY)
		m_pPlayer2Transform->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(m_pPlayer2Transform->GetPosition().x, m_BottomY);
	for (auto* enemy : m_EnemyTransforms)
	{
		if (enemy->GetPosition().y > m_BottomY)
			enemy->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(enemy->GetPosition().x, m_TopY);
		if (enemy->GetPosition().y < m_TopY)
			enemy->GetGameObject()->GetComponent<Box2DComponent>()->SetPosition(enemy->GetPosition().x, m_BottomY);
	}

	//  if both players are disabled/death, return to main menu
	if(!m_pPlayer->GetGameObject()->GetIsEnabled() && !m_pPlayer2->GetGameObject()->GetIsEnabled())
		SceneService.SetActiveScene(0);

	auto& gameTime = GameTimeService;
	if (m_GameMode != GameType::Versus)
	{
		// check if there are any enemies left enabled/not death
		for (auto* enemy : m_Enemies)
		{
			if (enemy->GetIsEnabled())
			{
				return;
			}
		}
	}
	else
	{
		// check if one of the two players is dead in case of the Versus game mode
		if (m_pPlayer->GetGameObject()->GetIsEnabled() && m_pPlayer2->GetGameObject()->GetIsEnabled())
			return;
	}

	// if no enemies left, update the timer
	m_Timer -= gameTime.GetElapsed();

	// if timer reached value, proceed to next scene
	if (m_Timer <= 0.f)
	{
		if (m_GameMode != GameType::Versus)
		{
			m_Player1Lives = m_pPlayer->GetLives();
			m_Player1Score = m_pPlayer->GetScore();
			m_Player2Lives = m_pPlayer2->GetLives();
			m_Player2Score = m_pPlayer2->GetScore();
			SceneService.SetActiveScene(m_NextScene);
			return;
		}
		// if Versus game mode there is no next scene, just return to main menu
		SceneService.SetActiveScene(0);
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
	m_pPlayer->Reset();
	m_pPlayer2->Reset();
	m_pPlayer->SetLives(m_Player1Lives);
	m_pPlayer->SetScore(m_Player1Score);
	m_pPlayer2->SetLives(m_Player2Lives);
	m_pPlayer2->SetScore(m_Player2Score);

	if (m_GameMode == GameType::SinglePlayer)
	{
		// disable player 2
		m_pPlayer2->GetGameObject()->SetIsEnabled(false);
		m_pPlayer2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(false);
		m_pPlayer2->SetIsMaita(false);
		m_Player2Lives = 0;
		m_pPlayer2->SetLives(m_Player2Lives);

		for (int i{}; i < m_Enemies.size(); i++)
		{
			ZenBehaviour* enemy = m_Enemies[i]->GetComponent<ZenBehaviour>();
			enemy->Reset();
			enemy->Set2PlayerMode(false);
		}
	}
	else if(m_GameMode == GameType::Coop)
	{
		// enable player 2 and set isMaita to false
		m_pPlayer2->GetGameObject()->SetIsEnabled(true);
		m_pPlayer2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(true);
		m_pPlayer2->SetIsMaita(false);
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
		m_pPlayer2->GetGameObject()->SetIsEnabled(true);
		m_pPlayer2->GetGameObject()->GetComponent<Box2DComponent>()->SetIsEnabled(true);
		m_pPlayer2->SetIsMaita(true);
		for (int i{}; i < m_Enemies.size(); i++)
		{
			// Disable all enemies in versus mode
			m_Enemies[i]->GetComponent<ZenBehaviour>()->Kill();
		}
	}
}

void LevelBehaviour::CreateZen(float xPos, float yPos, b2World* pPhysicsWorld, Scene* pScene)
{
	auto* go = new GameObject("Enemy");
	auto* zenBehaviour = new ZenBehaviour(go);
	zenBehaviour->Initialize(pPhysicsWorld, xPos, yPos, m_pPlayerTransform, m_pPlayer2Transform);
	go->AddComponent(zenBehaviour);
	pScene->Add(go);
	m_Enemies.push_back(go);
	m_EnemyTransforms.push_back(go->GetComponent<TransformComponent>());
}

void LevelBehaviour::CreateMaita(float xPos, float yPos, b2World* pPhysicsWorld, Scene* pScene)
{
	auto* go = new GameObject("Enemy");
	auto* maitaBehaviour = new MaitaBehaviour(go);
	maitaBehaviour->Initialize(pPhysicsWorld, xPos, yPos, m_pPlayerTransform, m_pPlayer2Transform);
	go->AddComponent(maitaBehaviour);
	pScene->Add(go);
	m_Enemies.push_back(go);
	m_EnemyTransforms.push_back(go->GetComponent<TransformComponent>());
}
