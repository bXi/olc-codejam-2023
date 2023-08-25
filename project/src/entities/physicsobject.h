#pragma once

#include "box2d/box2d.h"
#include "raylib.h"

#include "components/type.h"

class PhysicsObject
{
public:
    Color ShapeColor = RED;

    ObjectType type = ObjectType::UNDEFINED;

    b2Body* RigidBody = nullptr;
    virtual void draw() = 0;

    virtual ~PhysicsObject() = default;
};