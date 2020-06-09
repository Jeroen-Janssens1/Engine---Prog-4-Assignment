#pragma once
#include <string>
#include "State.h"
class RenderComponent;
class Animation final :
	public State
{
public:
	Animation(const std::string name, RenderComponent* pRenderComponent, std::vector<b2Vec2>& framePositions, float secondsPerFrame, bool isLooping = true, bool forceFullAnimation = false);
	~Animation()=default;

	void Update() override;
	void Entry() override;
	void Exit() override;

	bool HandleTransitions(State** newState) override;

private:
	unsigned int m_NrFrames;
	std::vector<b2Vec2> m_FramePositions;
	RenderComponent* m_pRenderComponent;
	float m_DurationPerFrame;
	float m_CurrentTime;
	size_t m_CurrentFrame;
	bool m_IsLooping;
	bool m_ForceFullAnimation;
	
};

