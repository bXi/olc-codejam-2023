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

class PhysicsRectangle : public PhysicsObject
{
public:
    Vector2 Size = { 0,0 };
    Vector2 HalfSize = { 0,0 };
    void draw() override;
    ObjectType type = ObjectType::RECTANGLE;
};

class PhysicsTriangle : public PhysicsObject
{
public:

    Vector2 Size = { 0,0 };
    Vector2 HalfSize = { 0,0 };
    void draw() override {

    };
    ObjectType type = ObjectType::TRIANGLE;
};
