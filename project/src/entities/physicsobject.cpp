#include "physicsobject.h"

#include "world/world.h"

void PhysicsRectangle::draw() {
        float scale = 64.f;//Configuration::tileWidth / World::getLevelScale();
        auto origin = vf2d{0.f, 0.f}; //(vf2d)HalfSize * 128.f;// * vi2d(Configuration::tileWidth, Configuration::tileHeight);

        Rectangle rec = {
                (RigidBody->GetPosition().x + .5f) * scale,
                (RigidBody->GetPosition().y + .5f) * scale,
                Size.x * scale,
                Size.y * scale
        };

        if (false) {
            b2Fixture *fixture = RigidBody->GetFixtureList();


            if (fixture) {
                // Assuming the fixture is indeed a box shape
                b2Shape *shape = fixture->GetShape();

                if (shape->GetType() == b2Shape::e_polygon) {
                    // If the shape is a polygon (box is a type of polygon)
                    auto *boxShape = dynamic_cast<b2PolygonShape *>(shape);

                    if (boxShape) {
                        // Now you have the box-shaped fixture, and you can work with it
                        vf2d v0 = (vf2d) boxShape->m_vertices[0] * scale;
                        vf2d v1 = (vf2d) boxShape->m_vertices[1] * scale;
                        vf2d v2 = (vf2d) boxShape->m_vertices[2] * scale;
                        vf2d v3 = (vf2d) boxShape->m_vertices[3] * scale;

                        DrawTriangle((vf2d) {rec.x, rec.y} + v0, (vf2d) {rec.x, rec.y} + v1, (vf2d) {rec.x, rec.y} + v2, GREEN);
                        DrawTriangle((vf2d) {rec.x, rec.y} + v0, (vf2d) {rec.x, rec.y} + v2, (vf2d) {rec.x, rec.y} + v3, GREEN);


                        DrawCircleV((vf2d) {rec.x, rec.y} + v0, 2.f, BLUE);
                        DrawCircleV((vf2d) {rec.x, rec.y} + v1, 2.f, BLUE);
                        DrawCircleV((vf2d) {rec.x, rec.y} + v2, 2.f, BLUE);
                        DrawCircleV((vf2d) {rec.x, rec.y} + v3, 2.f, BLUE);

                    }
                }
            }
        }





        DrawRectanglePro(rec, origin, 0.0f, ShapeColor);
    }