#pragma once
#include "ServiceLocator.h"
#include "Renderer.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "GameTime.h"
#include "SoundManager.h"
#include "PhysicsVariables.h"

#define RendererService ServiceLocator<Renderer, Renderer>::GetService()
#define InputService ServiceLocator<InputManager, InputManager>::GetService()
#define ResourceService ServiceLocator<ResourceManager, ResourceManager>::GetService()
#define SceneService ServiceLocator<SceneManager, SceneManager>::GetService()
#define GameTimeService ServiceLocator<GameTime, GameTime>::GetService()
#define SoundService ServiceLocator<SoundManager, SoundManager>::GetService()
#define PhysicsVariablesService ServiceLocator<PhysicsVariables, PhysicsVariables>::GetService()