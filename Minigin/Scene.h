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
	std::string tag;
};

	class Scene
	{
		friend Scene* SceneManager::CreateScene(const std::string& name, bool isTileMap, const std::string& levelDataPath);
	public:
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Add(GameObject* pObject);
		void Remove(GameObject* object);

		InputManager* GetInputManager() const { return m_pInputManager; }

		void OnLoad();
		void Update();
		void PhysicsUpdate();
		bool ProcessInput();
		void Render() const;

		std::string& GetName();
		b2World* GetPhysicsWorld();
		void SetGravity(const b2Vec2& gravity) { m_pPhysicsWorld->SetGravity(gravity); }

	private: 
		explicit Scene(const std::string& name, bool isTileMap, const std::string& levelDataPath);

		b2World* m_pPhysicsWorld;
		InputManager* m_pInputManager;
		
		std::string m_Name;
		std::vector <GameObject*> m_Objects{};
		std::vector <GameObject*> m_ToRemoveObjects{};
		std::vector<TileComponent*> m_TileMap;
		int m_CellWidth;
		int m_CellHeight;
		int m_NrCols;
		int m_NrRows;
		float m_TileWidth;
		float m_TileHeight;
		float m_PassedTime;
	};