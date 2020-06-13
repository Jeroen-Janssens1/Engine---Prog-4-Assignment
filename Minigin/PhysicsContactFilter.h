#pragma once
#include "Box2D.h"
#include "Box2DComponent.h"
class PhysicsContactFilter final : public b2ContactFilter
{
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
    {
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(fixtureA->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(fixtureB->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            return collider1->GetCollisionCallbackScript()->ShouldCollide(fixtureA, fixtureB, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            return collider2->GetCollisionCallbackScript()->ShouldCollide(fixtureA, fixtureB, collider2, collider1);
        return true;
    }
};