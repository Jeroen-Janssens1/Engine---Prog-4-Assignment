#pragma once
#include "BaseComponent.h"
#include <vector>
class Scene;
class Font;
class TransformComponent;
class MainMenu : public BaseComponent
{
public:
	MainMenu(GameObject* pOwner, Scene* scene);

	void Initialize(int width, int height, Font* font);

	void Update() override;

	void Render() const override;

	void SelectUp();
	void SelectDown();

	bool ConfirmSelection();

private:
	enum Buttons
	{
		SinglePlayer,
		Coop,
		Versus,
		Exit
	};

	void MoveSelector();

	Buttons m_SelectedButton;
	Scene* m_Scene;
	std::vector<TransformComponent*> m_Buttons;
	TransformComponent* m_SelectionIcon;
};

