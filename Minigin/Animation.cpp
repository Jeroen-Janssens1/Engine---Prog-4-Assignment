#include "MiniginPCH.h"
#include "Animation.h"
#include "RenderComponent.h"
#include "GameTime.h"

Animation::Animation(const std::string name, RenderComponent* pRenderComponent, std::vector<b2Vec2>& framePositions, float secondsPerFrame, bool isLooping, bool forceFullAnimation)
	:State(name)
	,m_pRenderComponent{pRenderComponent}
	,m_FramePositions{framePositions}
	,m_NrFrames{unsigned int(framePositions.size())}
	,m_DurationPerFrame{secondsPerFrame}
	,m_CurrentTime{0.f}
	,m_CurrentFrame{size_t(0)}
	,m_IsLooping{isLooping}
	,m_ForceFullAnimation{forceFullAnimation}
{
}

void Animation::Update()
{
	auto& gameTime = ServiceLocator<GameTime, GameTime>::GetService();
	m_CurrentTime += gameTime.GetElapsed();
	// while loop in case the duration per frame is so small that it is possible that a frame gets skipped in the animation
	while (m_CurrentTime >= m_DurationPerFrame)
	{
		// switch frames
		if (m_IsLooping)
			m_CurrentFrame = (m_CurrentFrame + 1) % m_NrFrames;
		else if (m_CurrentFrame != m_NrFrames - 1)
			m_CurrentFrame++;
		m_CurrentTime -= m_DurationPerFrame;
		m_pRenderComponent->SetSpritePos(int(m_FramePositions[m_CurrentFrame].x), int(m_FramePositions[m_CurrentFrame].y));
	}
}

void Animation::Entry()
{
	if(m_NrFrames > 0)
		m_pRenderComponent->SetSpritePos(int(m_FramePositions[m_CurrentFrame].x), int(m_FramePositions[m_CurrentFrame].y));
}

void Animation::Exit()
{
	State::Exit();
	m_CurrentTime = 0.f;
	m_CurrentFrame = 0;
}

bool Animation::HandleTransitions(State** newState)
{
	if (m_ForceFullAnimation && m_CurrentFrame != m_NrFrames - 1)
		return false;
	return State::HandleTransitions(newState);
}
