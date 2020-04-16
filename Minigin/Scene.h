#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "TileComponent.h"

struct TileType
{
	int tileId;
	int xPos;
	int yPos;
	bool usesPhysics;
	bool passableBottom;
	bool passableLeft;
	bool passableTop;
	bool passableRight;
};


// Tile has to be a component
// you add it to a gameobject!
//struct Tile
//{
//	int xPos;
//	int yPos;
//	RenderComponent* renderer;
//	TransformComponent* transform;
//};

namespace dae
{
	class Scene
	{
		friend std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight);
	public:
		void Add(const std::shared_ptr<GameObject>& object);

		void Update();
		void Render() const;
		std::string& GetName();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight);

		std::string m_Name;
		std::vector < std::shared_ptr<GameObject>> m_Objects{};
		std::vector<std::weak_ptr<TileComponent>> m_TileMap;
		int m_CellWidth;
		int m_CellHeight;
		int m_NrCols;
		int m_NrRows;
		float m_TileWidth;
		float m_TileHeight;

		static unsigned int m_IdCounter; 
	};

}
