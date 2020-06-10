#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "BinaryReader.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "GameTime.h"
#include "Box2DComponent.h"
#include "InputManager.h"
#include "PhysicsVariables.h"


unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, bool isTileMap, const std::string& levelDataPath) : m_Name(name)
{
	// Set up the physics for this scene
	m_pPhysicsWorld = new b2World(b2Vec2(0.f, 10.f));

	m_pInputManager = new InputManager();

	if (!isTileMap)
		return;
	// construct tile based info for this scene (tileMap vector) using the file
	BinaryReader binReader{};
	if (binReader.OpenFile(levelDataPath))
	{
		int nrOfTileTypes;
		binReader.Read<int>(nrOfTileTypes);

		std::string tileSheetPath;
		binReader.ReadString(tileSheetPath);

		binReader.Read<int>(m_CellWidth);
		binReader.Read<int>(m_CellHeight);

		binReader.Read<int>(m_NrCols);
		binReader.Read<int>(m_NrRows);
		int tileWidth{};
		binReader.Read<int>(tileWidth);
		m_TileWidth = float(tileWidth);
		int tileHeight;
		binReader.Read<int>(tileHeight);
		m_TileHeight = float(tileHeight);


		std::vector<TileType> tileTypes;
		tileTypes.resize(nrOfTileTypes);

		for (int i{}; i < nrOfTileTypes; i++)
		{
			// Sprite Sheet data
			binReader.Read<int>(tileTypes[i].tileId);
			if (tileTypes[i].tileId == 0)
				continue;

			binReader.Read<int>(tileTypes[i].xPos);
			binReader.Read<int>(tileTypes[i].yPos);

			// physics / collision data
			binReader.Read<bool>(tileTypes[i].usesPhysics);
		}

		for (int i{}; i < m_NrCols * m_NrRows; i++)
		{
			int tileId;
			binReader.Read<int>(tileId);
			// if tile id = 0 it means there is no tile in this 'slot' of the map, thus we can just skip and read the next tileId straight away
			if (tileId == 0)
				continue;

			// use tileId to create a tile of the correct type
			// at this position in the tile map
			// First get the xPos and yPos
			// make a gameObject
			// add a transform and render component
			// create the tile component
			// add all components to the gameobject
			// add the gameobject to the scene
			// add the tilecomponent to the tilemap
			int xPos = tileTypes[tileId].xPos;
			int yPos = tileTypes[tileId].yPos;
			auto go = new GameObject();
			TransformComponent* transformComponent = new TransformComponent{ go };
			go->AddComponent(transformComponent);
			RenderComponent* renderComponent = new RenderComponent{
				go, transformComponent, m_TileWidth, m_TileHeight, true, m_CellWidth, m_CellHeight, xPos, yPos };
			go->AddComponent(renderComponent);
			renderComponent->SetTexture(tileSheetPath);


			Add(go);

			int curRow = int(i) / m_NrCols;
			int curCol = (i - (m_NrCols*curRow)) % m_NrCols;
			// scale tiles according to window size
			float x = (curCol * m_TileWidth) + (m_TileWidth/2);
			float y = (curRow * m_TileHeight) + (m_TileHeight / 2);
			float z = 0;
			transformComponent->SetPosition(x, y, z);


			// Add The Physics Data
			if (tileTypes[tileId].usesPhysics)
			{
				auto collider = new Box2DComponent(go, transformComponent, m_pPhysicsWorld, m_TileWidth, m_TileHeight);
				go->AddComponent(collider);
			}
		}
	}
}

Scene::~Scene()
{
	for (size_t i{}; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] != ServiceLocator<GameTime, GameTime>::GetService().GetRenderingObject())
			delete m_Objects[i];
		m_Objects[i] = nullptr;
	}
	m_Objects.clear();
	m_TileMap.clear();
	delete m_pPhysicsWorld;
	delete m_pInputManager;
}

void Scene::Add(GameObject* object)
{
	m_Objects.push_back(object);
}

bool Scene::Update()
{
	// process scene specific input
	bool keepPlaying = m_pInputManager->ProcessInput();
	auto& physicsVars = ServiceLocator<PhysicsVariables, PhysicsVariables>::GetService();

	// update physics
	m_pPhysicsWorld->Step(physicsVars.GetTimeStep(), physicsVars.GetVelocityIterations(),
		physicsVars.GetPositionIterations());

	for(auto& object : m_Objects)
	{
		object->Update();
	}

	return keepPlaying;
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

std::string& Scene::GetName()
{
	return m_Name;
}

b2World* Scene::GetPhysicsWorld()
{
	return m_pPhysicsWorld;
}

