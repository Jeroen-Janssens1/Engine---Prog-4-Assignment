#pragma once
#include "SceneManager.h"
#include "PhysicsContactListener.h"
#include "PhysicsContactFilter.h"
class PhysicsVariables
{
public:
	PhysicsVariables();
	float GetPixelsPerUnit() const { return m_PixelsPerUnit; };
	float GetTimeStep() const { return m_TimeStep; };
	int32 GetVelocityIterations() const { return m_VelocityIterations; };
	int32 GetPositionIterations() const { return m_PositionIterations; };

	void SetPixelsPerUnit(float ppu) { m_PixelsPerUnit = ppu; };
	void SetTimeStep(float timeStep) { m_TimeStep = timeStep; };
	void SetVelocityIterations(int32 velocityIterations) { m_VelocityIterations = velocityIterations; };
	void SetPositionIterations(int32 positionIterations) { m_PositionIterations = positionIterations; };
	PhysicsContactListener* GetContactListener() const { return m_ContactListener; };
	PhysicsContactFilter* GetContactFilter() const { return m_ContactFilter; }
	void SetContactListener(PhysicsContactListener* listener) { m_ContactListener = listener; }
	void SetContactFilter(PhysicsContactFilter* filter) { m_ContactFilter = filter; }
	

private:
	float m_PixelsPerUnit;
	float m_TimeStep;
	int32 m_VelocityIterations;
	int32 m_PositionIterations;
	PhysicsContactListener* m_ContactListener;
	PhysicsContactFilter* m_ContactFilter;

};

