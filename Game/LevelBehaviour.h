#pragma once
#include "BaseComponent.h"
#include <string>
#include <vector>
class TextRenderComponent;
class GameObject;
class b2ContactListener;
class PlayerBehaviour;
class b2World;
class Scene;
class LevelBehaviour :
	public BaseComponent
{
public:
	enum class UIIcon
	{
		P1Score=0,
		P1Life,
		P2Score,
		P2Life
	};
	LevelBehaviour(GameObject* pOwner);
	~LevelBehaviour();

	void Initialize(const std::string& levelName, const std::string& levelPath, const std::string& nextScene);
	void Update() override;
	void Render() const override;
	void OnLoad() override;

private:
	void CreateZen(float xPos, float yPos, b2World* physicsWorld, Scene* scene);
	std::vector<GameObject*> m_Enemies;
	float m_Timer;
	std::string m_NextScene;
	PlayerBehaviour* m_Player;
	PlayerBehaviour* m_Player2;
	unsigned int m_PlayerLives;
	b2ContactListener* m_ContactListener;
	std::vector<TextRenderComponent*> m_UIRenders;

};

