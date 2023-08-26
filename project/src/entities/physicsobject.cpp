#include "physicsobject.h"

#include "world/world.h"

void PhysicsRectangle::draw() {
        float scale = Configuration::tileWidth / World::getLevelScale();
        auto origin = (vf2d)HalfSize;// * vi2d(Configuration::tileWidth, Configuration::tileHeight);
        Rectangle rec = { RigidBody->GetPosition().x * scale,RigidBody->GetPosition().y * scale, Size.x, Size.y };
        DrawRectanglePro(rec, {0.0f, 0.0f}, 0.0f, ShapeColor);
    }