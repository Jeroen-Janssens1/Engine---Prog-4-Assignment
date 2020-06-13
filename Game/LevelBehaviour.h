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
class TransformComponent;
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
	enum class GameType
	{
		SinglePlayer=0,
		Coop,
		Versus
	};
	LevelBehaviour(GameObject* pOwner);
	~LevelBehaviour();

	void Initialize(const std::string& levelName, const std::string& levelPath, const std::string& nextScene);
	void Update() override;
	void Render() const override;
	void OnLoad() override;

	static void SetGameType(GameType mode) { m_GameMode = mode; }
	static void SetPlayer1Lives(unsigned int value) { m_Player1Lives = value; }
	static void SetPlayer1Score(unsigned int value) { m_Player1Score = value; }
	static void SetPlayer2Lives(unsigned int value) { m_Player2Lives = value; }
	static void SetPlayer2Score(unsigned int value) { m_Player2Score = value; }

private:
	void CreateZen(float xPos, float yPos, b2World* physicsWorld, Scene* scene);
	std::vector<GameObject*> m_Enemies;
	float m_Timer;
	std::string m_NextScene;
	PlayerBehaviour* m_Player;
	TransformComponent* m_PlayerTransform;
	TransformComponent* m_Player2Transform;
	PlayerBehaviour* m_Player2;
	b2ContactListener* m_ContactListener;
	std::vector<TextRenderComponent*> m_UIRenders;
	static GameType m_GameMode;

	static unsigned int m_Player1Lives;
	static unsigned int m_Player2Lives;
	static unsigned int m_Player1Score;
	static unsigned int m_Player2Score;
};

