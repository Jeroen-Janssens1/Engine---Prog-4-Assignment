#pragma once
#include "InputManager.h"
#include "GameTime.h"
#include "GameObject.h"
#include <memory>
#include "Command.h"
class TransformComponent;
class PlayerPrefab final :
	public GameObject
{
public:
	PlayerPrefab();
	~PlayerPrefab() = default;
	
	void Initialize();
	void Update() override;



	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

private:
	TransformComponent* m_Transform;
	InputManager& m_Input;
	GameTime& m_GameTime;
};