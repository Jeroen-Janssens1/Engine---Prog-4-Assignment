#include "MainMenu.h"
#include "GameObject.h"
#include "Scene.h"
#include "Font.h"
#include "Services.h"
#include "InputCommands.h"
#include "LevelBehaviour.h"

MainMenu::MainMenu(GameObject* pOwner, Scene* pScene)
	:BaseComponent(pOwner)
	,m_pScene{pScene}
{
}

void MainMenu::Initialize(int width, int height, Font* pFont)
{
	// Single Player Button
	m_SelectedButton = Buttons::SinglePlayer;
	float offset = 200.f;
	float gapSize = (height - (offset * 2)) / 4;
	auto* go = new GameObject();
	auto* tc = new TransformComponent(go);
	auto* textRenderComp = new TextRenderComponent(go, tc, "SINGLE PLAYER", pFont, {255, 255, 0});
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset, 0.f);
	m_pScene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	// CO-OP Button
	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "CO-OP", pFont);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + gapSize, 0.f);
	m_pScene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	// Versus Button
	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "VERSUS", pFont);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + (gapSize * 2), 0.f);
	m_pScene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	// Exit Button
	go = new GameObject();
	tc = new TransformComponent(go);
	textRenderComp = new TextRenderComponent(go, tc, "EXIT", pFont);
	go->AddComponent(tc);
	go->AddComponent(textRenderComp);
	tc->SetPosition(width / 2.f, offset + (gapSize * 3), 0.f);
	m_pScene->Add(go);
	m_Buttons.push_back(tc);
	m_ButtonRenders.push_back(textRenderComp);

	// Selection Icon
	go = new GameObject();
	m_pSelectionIcon = new TransformComponent(go);
	auto* rc = new RenderComponent(go, m_pSelectionIcon, "", 8, 8, true, 6, 8, 20, 40);
	rc->SetTexture("Resources/Player1.png");
	go->AddComponent(m_pSelectionIcon);
	go->AddComponent(rc);
	tc = m_Buttons[int(m_SelectedButton)];
	m_pSelectionIcon->SetPosition((width / 2.f) - 60.f, tc->GetPosition().y, 0.f);
	m_pScene->Add(go);
	
	auto* input = m_pScene->GetInputManager();
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
	// Reset selection to Single Player
	m_ButtonRenders[int(m_SelectedButton)]->SetColor({ 255, 255, 255 });
	m_SelectedButton = Buttons::SinglePlayer;
	MainMenu::MoveSelector();
	m_ButtonRenders[int(m_SelectedButton)]->SetColor({ 255, 255, 0 });
	SoundService.EditSound(0, SoundManager::Action::Stop);
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
	// Set static level variables to default values
	LevelBehaviour::SetPlayer1Lives(4);
	LevelBehaviour::SetPlayer2Lives(4);
	LevelBehaviour::SetPlayer1Score(0);
	LevelBehaviour::SetPlayer2Score(0);
	switch (m_SelectedButton)
	{
	case Buttons::SinglePlayer:
		LevelBehaviour::SetGameType(LevelBehaviour::GameType::SinglePlayer);
		SoundService.EditSound(0, SoundManager::Action::Play);
		SceneService.SetActiveScene(1);
		return true;
		break;
	case Buttons::Coop:
		LevelBehaviour::SetGameType(LevelBehaviour::GameType::Coop);
		SoundService.EditSound(0, SoundManager::Action::Play);
		SceneService.SetActiveScene(1);
		return true;
		break;
	case Buttons::Versus:
		LevelBehaviour::SetGameType(LevelBehaviour::GameType::Versus);
		SoundService.EditSound(0, SoundManager::Action::Play);
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
	m_pSelectionIcon->SetPosition(m_pSelectionIcon->GetPosition().x, m_Buttons[int(m_SelectedButton)]->GetPosition().y, 0.f);
}
