#pragma once
#include "Singleton.h"
#include <chrono>
class GameObject;
class TextRenderComponent;
class GameTime final : public dae::Singleton<GameTime>
{
public:
#define TimePoint std::chrono::steady_clock::time_point

	void Init();
	void Update();
	void Render();
	void Reset();

	std::shared_ptr<GameObject>& GetRenderingObject();

private:
	friend class Singleton<GameTime>;
	GameTime() = default;

	float m_DeltaTime;
	TimePoint m_PrevTime;
	TimePoint m_CurTime;
	int m_FPS;
	float m_FPSTimer;
	int m_FPSCount;

	std::shared_ptr<GameObject> m_pGameObject;

	std::shared_ptr<TextRenderComponent> m_pTextRenderer;
};

