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
			binReader.Read<bool>(tileTypes[i].usesPhysics);
			binReader.ReadString(tileTypes[i].tag);
		}
		std::vector<int> map{};
		for (int i{}; i < m_NrCols * m_NrRows; i++)
		{
			int tileId;
			binReader.Read<int>(tileId);
			map.push_back(tileId);
			// if tile id = 0 it means there is no tile in this 'slot' of the map, thus we can just skip and read the next tileId straight away
			if (tileId == 0)
				continue;

			int xPos = tileTypes[tileId].xPos;
			int yPos = tileTypes[tileId].yPos;
			auto go = new GameObject(tileTypes[tileId].tag);
			TransformComponent* transformComponent = new TransformComponent{ go };
			go->AddComponent(transformComponent);
			RenderComponent* renderComponent = new RenderComponent{
				go, transformComponent, "", m_TileWidth, m_TileHeight, true, m_CellWidth, m_CellHeight, xPos, yPos };
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
				//auto collider = new Box2DComponent(go, transformComponent, m_pPhysicsWorld, m_TileWidth, m_TileHeight);
				//go->AddComponent(collider);
			}
		}

		// create joint collision boxes
		std::vector<bool> tileUsed{};
		tileUsed.resize(map.size(), false);

		for (int i{}; i < m_NrRows; i++)
		{
			for (int j{}; j < m_NrCols; j++)
			{
				// i*nrOfCols + j = indx
				int indx = (i * m_NrCols) + j;
				if (map[indx] == 0 || !tileTypes[map[indx]].usesPhysics || tileUsed[indx])
					continue;

				// this gives us the top left pos of the collision box
				int reqId = map[indx];
				tileUsed[indx] = true;
				// now we need the top right and bottom left positions to get the width and height, then we have all the neccesary info to
				// create a collision box for these tiles
				int topLeft = indx;
				int width = 1;
				indx++;
				// check to the right of the tile until not possible anymore, this will give us the width of the collision box in nr of cols
				for (int k{ j + 1 }; k < m_NrCols + 1; k++)
				{
					indx = (i * m_NrCols) + k;
					if (k == m_NrCols)
						break;
					// we only add tiles of the same id.
					if (map[indx] != reqId || tileUsed[indx])
						break;
					tileUsed[indx] = true; // add tile to this collision box
				}
				width = indx - topLeft;


				int height = 1;
				// check downwards to get the bottom left pos
				for (int k{ i + 1 }; k < m_NrRows + 1; k++)
				{
					if (k == m_NrRows)
					{
						height = (k - i);
						break;
					}
					indx = (k * m_NrCols) + j;
					indx++;
					// check to the right
					for (int l{ j }; l < m_NrCols + 1; l++)
					{
						indx = (k * m_NrCols) + l;
						if (l == m_NrCols)
							break;
						if (map[indx] != reqId || tileUsed[indx] || indx == (k * m_NrCols) + width)
							break;
					}
					// if we managed to reach the required col in this row
					if (indx == (k * m_NrCols) + j + width)
					{
						// set all tiles from this row (that are in the collision box) to used
						for (int l{ (k * m_NrCols) + j }; l < indx; l++)
						{
							tileUsed[l] = true;
						}
					}
					else
					{
						// retrieve the height
						height = (k - i);
						break;
					}
				}

				// we now have the top left position index and the width and height in nr of tiles
				// we now need to calculate the top left pos in pixel coordinates and the width and height in pixels
				float actualWidth = width * m_TileWidth;
				float actualHeight = height * m_TileHeight;
				float topLeftX = j * m_TileWidth;
				float topLeftY = i * m_TileHeight;

				// now we still need to calculate the center of the collision box
				float centerX = topLeftX + (actualWidth / 2.f);
				float centerY = topLeftY + (actualHeight / 2.f);

				// create the game object with the collision box and correct tags and add it to the scene
				auto* go = new GameObject(tileTypes[reqId].tag);
				auto* tc = new TransformComponent(go);
				tc->SetPosition(centerX, centerY, 0.f);
				auto* box2DComp = new Box2DComponent(go, tc, m_pPhysicsWorld, actualWidth, actualHeight);
				go->AddComponent(box2DComp);
				go->AddComponent(tc);
				Add(go);

				// normally level 1 would create 14 collisions using this system
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

void Scene::OnLoad()
{
	for (auto& object : m_Objects)
	{
		object->OnLoad();
	}
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
		if(object->GetIsEnabled())
			object->Update();
	}

	return keepPlaying;
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		if(object->GetIsEnabled())
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

