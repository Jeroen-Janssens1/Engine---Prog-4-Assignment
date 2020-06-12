#pragma once
#include "Box2D.h"
class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
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

    void EndContact(b2Contact* contact)
    {
        Box2DComponent* collider1 = static_cast<Box2DComponent*>(contact->GetFixtureA()->GetUserData());
        Box2DComponent* collider2 = static_cast<Box2DComponent*>(contact->GetFixtureB()->GetUserData());
        if (collider1->GetCollisionCallbackScript())
            collider1->GetCollisionCallbackScript()->OnContactEnd(contact, collider1, collider2);
        if (collider2->GetCollisionCallbackScript())
            collider2->GetCollisionCallbackScript()->OnContactEnd(contact, collider2, collider1);
    }
};

