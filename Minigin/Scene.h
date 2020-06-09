#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "TileComponent.h"

class InputManager;
class b2World;
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

namespace dae
{
	class Scene
	{
		friend Scene* SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight);
	public:

		void Add(GameObject* pObject);

		InputManager* GetInputManager() const { return m_pInputManager; }

		void Update();
		void Render() const;
		std::string& GetName();
		b2World* GetPhysicsWorld();

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name, bool isTileMap, const std::string& levelDataPath, int windowWidth, int windowHeight);

		b2World* m_pPhysicsWorld;
		InputManager* m_pInputManager;
		
		std::string m_Name;
		std::vector <GameObject*> m_Objects{};
		std::vector<TileComponent*> m_TileMap;
		int m_CellWidth;
		int m_CellHeight;
		int m_NrCols;
		int m_NrRows;
		float m_TileWidth;
		float m_TileHeight;

		static unsigned int m_IdCounter; 
	};

}
