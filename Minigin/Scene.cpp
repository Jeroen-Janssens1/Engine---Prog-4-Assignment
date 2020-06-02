#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "BinaryReader.h"
#include "RenderComponent.h"
#include "TransformComponent.h"
#include "GameTime.h"

using namespace dae;

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight) : m_Name(name)
{
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

		std::vector<TileType> tileTypes;
		tileTypes.resize(nrOfTileTypes);

		for (int i{}; i < nrOfTileTypes; i++)
		{
			// Sprite Sheet data
			binReader.Read<int>(tileTypes[i].tileId);
			binReader.Read<int>(tileTypes[i].xPos);
			binReader.Read<int>(tileTypes[i].yPos);

			// physics / collision data
			binReader.Read<bool>(tileTypes[i].usesPhysics);
			if (tileTypes[i].usesPhysics)
			{
				binReader.Read<bool>(tileTypes[i].passableBottom);
				binReader.Read<bool>(tileTypes[i].passableTop);
				binReader.Read<bool>(tileTypes[i].passableLeft);
				binReader.Read<bool>(tileTypes[i].passableRight);
			}
		}

		m_TileMap.resize(m_NrCols * m_NrRows);

		for (size_t i{}; i < m_TileMap.size(); i++)
		{
			int tileId;
			binReader.Read<int>(tileId);

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
			m_TileWidth = float(windowWidth) / m_NrCols;
			m_TileHeight = float(windowHeight) / m_NrRows;
			RenderComponent* renderComponent = new RenderComponent{
				go, transformComponent, m_TileWidth, m_TileHeight, true, 10, 10, m_CellWidth, m_CellHeight, xPos, yPos };
			go->AddComponent(renderComponent);
			TileComponent* tileComponent = new TileComponent(xPos, yPos, renderComponent, transformComponent, go, tileSheetPath);
			go->AddComponent(tileComponent);

			Add(go);
			m_TileMap[i] = tileComponent;
			// 5x6 map
			// if i = 5, row = 1
			// take i/nrCols and ground it
			// if i = 9, 9/6 = 1.8, grounded = 1
			// for col, i/nrCols % nrCols;
			// now col = 9/6 % 6 = 4
			// calculate the actual position for the transform component using the location in the tilemap together
			// with the window width and height

			int curRow = int(i) / m_NrCols;
			int curCol = (i - (m_NrCols*curRow)) % m_NrCols;
			// scale tiles according to window size
			float x = (curCol * m_TileWidth);
			float y = (curRow * m_TileHeight);
			float z = 0;
			transformComponent->SetPosition(x, y, z);


			// All of the collision data will be held by the
			// collision and physics object components
			// which currently aren't added yet!
		}
	}

	


}

Scene::~Scene()
{
	/*for (int i{}; i < m_TileMap.size(); i++)
	{
		delete m_TileMap[i];
		m_TileMap[i] = nullptr;
	}*/
	//m_TileMap.clear();
	for (int i{}; i < m_Objects.size(); i++)
	{
		if (m_Objects[i] != ServiceLocator<GameTime, GameTime>::GetService().GetRenderingObject())
			delete m_Objects[i];
		m_Objects[i] = nullptr;
	}
	m_Objects.clear();
}

void Scene::Add(GameObject* object)
{
	m_Objects.push_back(object);
}

void Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

std::string& dae::Scene::GetName()
{
	return m_Name;
}

