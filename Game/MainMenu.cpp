#include "MainMenu.h"
#include "GameObject.h"
#include "Scene.h"
#include "Font.h"
#include "Services.h"
#include "InputCommands.h"

MainMenu::MainMenu(GameObject* pOwner, Scene* scene)
	:BaseComponent(pOwner)
	,m_Scene{scene}
{
}

void MainMenu::Initialize(int width, int height, Font* font)
{
	m_SelectedButton = Buttons::SinglePlayer;
	float offset = 200.f;
	float gapSize = (height - (offset * 2)) / 4;
	auto* go = new GameObject();
	auto* tc = new TransformComponent(go);
	auto* textRenderComp = new TextRenderComponent(go, tc, "SINGLE PLAYER", font, {255, 255, 0});
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset, 0.f);
	m_Scene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "CO-OP", font);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + gapSize, 0.f);
	m_Scene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "VERSUS", font);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + (gapSize * 2), 0.f);
	m_Scene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "EXIT", font);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + (gapSize * 3), 0.f);
	m_Scene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	go = new GameObject();
	m_SelectionIcon = new TransformComponent(go);
	auto* rc = new RenderComponent(go, m_SelectionIcon, 8, 8, true, 6, 8, 20, 40);
	rc->SetTexture("Resources/Player1.png");
	go->AddComponent(m_SelectionIcon);
	go->AddComponent(rc);
	tc = m_Buttons[int(m_SelectedButton)];
	m_SelectionIcon->SetPosition((width / 2.f) - 60.f, tc->GetPosition().y, 0.f);
	m_Scene->Add(go);

	auto* input = m_Scene->GetInputManager();

	input->MapCommand(VK_UP, new MenuSelectUpCommand(this), true);
	input->MapCommand(VK_DOWN, new MenuSelectDownCommand(this), true);
	input->MapCommand(VK_RETURN, new MenuConfirmSelectionCommand(this), true);

	input->MapCommand(0, ControllerButton::DPadUp, new MenuSelectUpCommand(this), true);
	input->MapCommand(0, ControllerButton::DPadDown, new MenuSelectDownCommand(this), true);
	input->MapCommand(0, ControllerButton::ButtonA, new MenuConfirmSelectionCommand(this), true);
}

void MainMenu::Update()
{
}

void MainMenu::Render() const
{

}

void MainMenu::OnLoad()
{
	m_ButtonRenders[int(m_SelectedButton)]->SetColor({ 255, 255, 255 });
	m_SelectedButton = Buttons::SinglePlayer;
	MainMenu::MoveSelector();
	m_ButtonRenders[int(m_SelectedButton)]->SetColor({ 255, 255, 0 });
}

void MainMenu::SelectUp()
{
	m_SelectedButton = Buttons((int(m_SelectedButton) - 1) % m_Buttons.size());
	MoveSelector();
	m_ButtonRenders[int(m_SelectedButton)]->SetColor(SDL_Color{ 255, 255, 0 });
	if (int(m_SelectedButton) == m_Buttons.size() - 1)
		m_ButtonRenders.front()->SetColor(SDL_Color{ 255, 255, 255 });
	else
		m_ButtonRenders[int(m_SelectedButton) + 1]->SetColor(SDL_Color{ 255, 255, 255 });
}

void MainMenu::SelectDown()
{
	m_SelectedButton = Buttons((int(m_SelectedButton) + 1) % m_Buttons.size());
	MoveSelector();
	m_ButtonRenders[int(m_SelectedButton)]->SetColor(SDL_Color{ 255, 255, 0 });
	if (int(m_SelectedButton) == 0)
		m_ButtonRenders.back()->SetColor(SDL_Color{ 255, 255, 255 });
	else
		m_ButtonRenders[int(m_SelectedButton) - 1]->SetColor(SDL_Color{ 255, 255, 255 });
}

bool MainMenu::ConfirmSelection()
{
	switch (m_SelectedButton)
	{
	case Buttons::SinglePlayer:
		SceneService.SetActiveScene(1);
		return true;
		break;
	case Buttons::Coop:
		SceneService.SetActiveScene(1);
		return true;
		break;
	case Buttons::Versus:
		SceneService.SetActiveScene(1);
		return true;
		break;
	case Buttons::Exit:
		return false;
		break;
	}

	return true;
}

void MainMenu::MoveSelector()
{
	m_SelectionIcon->SetPosition(m_SelectionIcon->GetPosition().x, m_Buttons[int(m_SelectedButton)]->GetPosition().y, 0.f);
}
