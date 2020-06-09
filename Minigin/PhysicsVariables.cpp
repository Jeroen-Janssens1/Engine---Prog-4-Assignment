#include "MiniginPCH.h"
#include "PhysicsVariables.h"

PhysicsVariables::PhysicsVariables()
	:m_PixelsPerUnit{32}
	,m_TimeStep{1.f / 60.f}
	,m_VelocityIterations{6}
	,m_PositionIterations{2}
{
}