#pragma once
#include "InputManager.h"
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
	
	void Initialize(std::shared_ptr<GameObject> thisSmart);
	void Update() override;



	void MoveUp();
	void MoveDown();
	void MoveRight();
	void MoveLeft();

private:
	std::shared_ptr<TransformComponent> m_Transform;
	InputManager& m_Input;
};