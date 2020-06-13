#pragma once
#include "Box2D.h"
#include "Box2DComponent.h"

class PhysicsContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override
    {
        //check if fixture A was the foot sensor
        // we know for a fact that user data is always a Box2DComponent pointer
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            collider1->GetCollisionCallbackScript()->OnContactBegin(contact, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            collider2->GetCollisionCallbackScript()->OnContactBegin(contact, collider2, collider1);
    }

    void EndContact(b2Contact* contact) override
    {
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            collider1->GetCollisionCallbackScript()->OnContactEnd(contact, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            collider2->GetCollisionCallbackScript()->OnContactEnd(contact, collider2, collider1);
    }

    void PreSolve(b2Contact* contact, const b2Manifold* manifold) override
    {
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            collider1->GetCollisionCallbackScript()->PreSolve(contact, manifold, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            collider2->GetCollisionCallbackScript()->PreSolve(contact, manifold, collider2, collider1);
    }

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
    {
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            collider1->GetCollisionCallbackScript()->PostSolve(contact, impulse, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            collider2->GetCollisionCallbackScript()->PostSolve(contact, impulse, collider2, collider1);
    }
};