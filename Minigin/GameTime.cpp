#include "MiniginPCH.h"
#include "GameTime.h"
#include "ResourceManager.h"
#include "TransformComponent.h"
#include "TextRenderComponent.h"
#include "GameObject.h"


void GameTime::Init()
{
	m_DeltaTime = 0.f;
	m_PrevTime = std::chrono::high_resolution_clock::now();
	m_CurTime = std::chrono::high_resolution_clock::now();
	m_FPS = 0;
	m_FPSTimer = 0.f;
	m_FPSCount = 0;

	auto font = dae::ResourceManager::GetInstance().LoadFont("../Data/Lingua.otf", 24);

	m_pGameObject = std::make_shared<GameObject>();

	auto transformComponent = std::make_shared<TransformComponent>(TransformComponent{ m_pGameObject });
	auto addComponent = std::static_pointer_cast<BaseComponent>(transformComponent);
	m_pGameObject->AddComponent(addComponent);

	m_pTextRenderer = std::make_shared<TextRenderComponent>
		(TextRenderComponent{ m_pGameObject, transformComponent, "0 FPS", font });
	addComponent = std::static_pointer_cast<BaseComponent>(m_pTextRenderer);
	m_pGameObject->AddComponent(addComponent);

	transformComponent->SetPosition(0, 0, 0);
}

void GameTime::Update()
{
	TimePoint currTime = std::chrono::high_resolution_clock::now();
	m_CurTime = currTime;
	
	std::chrono::duration<float> diff = m_CurTime - m_PrevTime;
	m_DeltaTime = diff.count();
	m_PrevTime = m_CurTime;
	
	if (m_DeltaTime < 0.f)
		m_DeltaTime = 0.f;

	m_FPSTimer += m_DeltaTime;
	m_FPSCount++;
	if (m_FPSTimer > 1.f)
	{
		m_FPSTimer -= 1.f;
		m_FPS = m_FPSCount;
		m_FPSCount = 0;
		m_pTextRenderer->SetText(std::to_string(m_FPS) + " FPS");
	}
	
}

void GameTime::Render()
{

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

std::shared_ptr<GameObject>& GameTime::GetRenderingObject()
{
	return m_pGameObject;
}

float GameTime::GetElapsed()
{
	return m_DeltaTime;
}
