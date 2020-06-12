#pragma once
#include "BaseComponent.h"
#include <string>
#include <vector>
#include "GameObject.h"
#include "Box2D.h"
class PlayerBehaviour;
class ContactListener;
class LevelBehaviour :
	public BaseComponent
{
public:
	LevelBehaviour(GameObject* pOwner);
	~LevelBehaviour();

	void Initialize(const std::string& levelName, const std::string& levelPath, const std::string& nextScene);
	void Update() override;
	void Render() const override;
	void OnLoad() override;

private:
	std::vector<GameObject*> m_Enemies;
	float m_Timer;
	std::string m_NextScene;
	PlayerBehaviour* m_Player;
	b2ContactListener* m_ContactListener;

};

