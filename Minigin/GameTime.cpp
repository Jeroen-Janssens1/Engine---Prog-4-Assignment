#include "MiniginPCH.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "TextRenderComponent.h"
#include "GameObject.h"
#include "ServiceLocator.h"


GameTime::~GameTime()
{
	delete m_pGameObject;
}

void GameTime::Init()
{
	m_DeltaTime = 0.f;
	m_PrevTime = std::chrono::high_resolution_clock::now();
	m_CurTime = std::chrono::high_resolution_clock::now();
	m_FPS = 0;
	m_FPSTimer = 0.f;
	m_FPSCount = 0;

	auto font = ServiceLocator<ResourceManager, ResourceManager>::GetService().LoadFont("Resources/Lingua.otf", 24);
	m_pGameObject = new GameObject();
	auto transformComponent = new TransformComponent{ m_pGameObject };
	m_pGameObject->AddComponent(transformComponent);
	m_pTextRenderer = new TextRenderComponent(m_pGameObject, transformComponent, "0 FPS", font);
	m_pGameObject->AddComponent(m_pTextRenderer);
	transformComponent->SetPosition(0, 0, 0);
}

void GameTime::Update()
{
	m_CurTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> diff = m_CurTime - m_PrevTime;
	m_DeltaTime = diff.count();
	m_FPSTimer += m_DeltaTime;
	m_FPSCount++;
	if (m_FPSTimer > 1.f)
	{
		m_FPSTimer -= 1.f;
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_pTextRenderer->SetText(std::to_string(m_FPS) + " FPS");
	}
	m_PrevTime = m_CurTime;
}

void GameTime::Reset()
{
	m_DeltaTime = 0;
	m_PrevTime = TimePoint{};
	m_CurTime = TimePoint{};
	m_FPS = 0;
	m_FPSTimer = 0;
	m_FPSCount = 0;
}

void GameTime::SetFont(const std::string& path)
{
	auto font = ServiceLocator<ResourceManager, ResourceManager>::GetService().LoadFont(path, 36);
	m_pTextRenderer->SetFont(font);
}

void GameTime::SetFontSize(unsigned int size)
{
	m_pTextRenderer->SetFontSize(size);
}

GameObject* GameTime::GetRenderingObject()
{
	return m_pGameObject;
}

float GameTime::GetElapsed()
{
	return m_DeltaTime;
}
