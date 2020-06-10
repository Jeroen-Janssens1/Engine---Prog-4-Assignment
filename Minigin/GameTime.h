#pragma once
#include <chrono>
#include <string>
class GameObject;
class TextRenderComponent;
class GameTime final
{
public:
#define TimePoint std::chrono::steady_clock::time_point
	GameTime() = default;
	~GameTime();

	void Init();
	void Update();
	void Render();
	void Reset();

	void SetFont(const std::string& path);
	void SetFontSize(unsigned int size);

	GameObject* GetRenderingObject();

	float GetElapsed();

private:
	float m_DeltaTime;
	TimePoint m_PrevTime;
	TimePoint m_CurTime;
	int m_FPS;
	float m_FPSTimer;
	int m_FPSCount;

	GameObject* m_pGameObject;

	TextRenderComponent* m_pTextRenderer;
};

