#include "world.h"

#include "ecs.h"
#include "components/health.h"

#include "components/owner.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"

void World::_loadLevelPhysics(LevelData ld)
{

    using json = nlohmann::json;


    std::ifstream f("assets/tilesets/tileset.tsj");
    json data = json::parse(f);

    std::unordered_map<int, Shape*> uniqueShapes;

    // read all unique shapes into our uniqueShapes map.
    for (auto& tile : data["tiles"]) {
        json jsonobject = tile["objectgroup"]["objects"][0];

        int id = tile["id"];

        if (jsonobject.contains("polygon")) { //assume triangle
            auto* t = new wTriangle;

            t->type = Shape::TRIANGLE;
            t->first  = {jsonobject["polygon"][0]["x"], jsonobject["polygon"][0]["y"]};
            t->second = {jsonobject["polygon"][1]["x"], jsonobject["polygon"][1]["y"]};
            t->third  = {jsonobject["polygon"][2]["x"], jsonobject["polygon"][2]["y"]};

            uniqueShapes[id] = t;
        } else { //assume rectangle
            auto* r = new wRectangle;

            r->type = Shape::RECTANGLE;

            r->x = jsonobject["x"];
            r->y = jsonobject["y"];
            r->width = jsonobject["width"];
            r->height = jsonobject["height"];

            uniqueShapes[id] = r;
        }
    }

    for (int x = 0; x < ld.width; x++) {
        for (int y = 0; y < ld.height; y++) {
            int index = y * ld.width + x;

            int tileId = ld.floorTiles.at(index);
            if (tileId == 0) continue;

            float offSetX = (float) x;
            float offSetY = (float) y;

            Shape* shape = uniqueShapes[tileId - 1];

            if (shape->type == Shape::RECTANGLE) {

                wRectangle* rect = (wRectangle*)shape;

                PhysicsRectangle* r = new PhysicsRectangle();

                r->ShapeColor = Color{230, 41, 55, 127};

                r->Size.x = rect->width / (256.f / levelScale);
                r->Size.y = rect->height / (256.f / levelScale);

                r->HalfSize.x = (r->Size.x / 2);
                r->HalfSize.y = (r->Size.y / 2);

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position.Set(
                        (offSetX - 0.5f) + (rect->x / (128.f)),
                        (offSetY - 0.5f) + (rect->y / (128.f))
                );
                bodyDef.angle = 0.0f;
                bodyDef.userData.pointer = 0;
                r->RigidBody = World::createBody(&bodyDef);

                b2PolygonShape Box;

                b2Vec2 center = {
                    (r->HalfSize.x),
                    (r->HalfSize.y)
                };

                Box.SetAsBox(r->HalfSize.x, r->HalfSize.y, center, 0.f);


                b2FixtureDef fixtureDef;

                fixtureDef.shape = &Box;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.0f;

                r->RigidBody->CreateFixture(&fixtureDef);

                addObject(r);
            } else if (shape->type == Shape::TRIANGLE) {





                wTriangle* rect = (wTriangle*)shape;

                PhysicsTriangle* t = new PhysicsTriangle();

                t->ShapeColor = Color{41, 55, 230, 127};

                t->Size.x = 1.0f;
                t->Size.y = 1.0f;

                t->HalfSize.x = (t->Size.x / 2);
                t->HalfSize.y = (t->Size.y / 2);

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position.Set(
                        (offSetX - 0.5f),
                        (offSetY - 0.5f)
                );
                bodyDef.angle = 0.0f;
                bodyDef.userData.pointer = 0;
                t->RigidBody = World::createBody(&bodyDef);



                b2PolygonShape dynamicPolygon;
                b2Vec2 vertices[3];
                if (tileId - 1 == 19) {
                    vertices[0].Set(0.f, 1.f);
                    vertices[1].Set(1.f, 1.f);
                    vertices[2].Set(1.f, 0.f);
                } else {
                    vertices[0].Set(0.f, 0.f);
                    vertices[1].Set(0.f, 1.f);
                    vertices[2].Set(1.f, 1.f);
                }
                dynamicPolygon.Set(vertices, 3);








                b2FixtureDef fixtureDef;

                fixtureDef.shape = &dynamicPolygon;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.0f;

                t->RigidBody->CreateFixture(&fixtureDef);

                addObject(t);





            }



        }
    }
}

void World::_init()
{

}

void World::handleCollision(b2Contact* contact, bool beginCollision)
{

}

void World::_removeObject(PhysicsObject* object)
{

    auto it = std::find(objects.begin(), objects.end(), object);
    if (it != objects.end()) {
        b2Body* body = object->RigidBody;

        // Delete the fixtures attached to the body
        b2Fixture* fixture = body->GetFixtureList();
        while (fixture) {
            b2Fixture* nextFixture = fixture->GetNext();
            body->DestroyFixture(fixture);
            fixture = nextFixture;
        }

        body->GetWorld()->DestroyBody(body);

        objects.erase(it);
        delete object;
    }

}

void World::BeginContact(b2Contact* contact)
{
    // Get the fixtures involved in the contact
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();

    // Get the bodies associated with the fixtures
    b2Body* bodyA = fixtureA->GetBody();
    b2Body* bodyB = fixtureB->GetBody();


    b2Shape* shapeA = fixtureA->GetShape();
    b2Shape* shapeB = fixtureB->GetShape();

    // Calculate the size of bodyA
    b2AABB aabbA;
    shapeA->ComputeAABB(&aabbA, bodyA->GetTransform(), 0); // 0 is the child index
    vf2d sizeA = (vf2d)aabbA.GetExtents() * 2.0f;

    // Calculate the size of bodyB
    b2AABB aabbB;
    shapeB->ComputeAABB(&aabbB, bodyB->GetTransform(), 0); // 0 is the child index
    vf2d sizeB = (vf2d)aabbB.GetExtents() * 2.0f;

    bool bodyAIsAboveBodyB = bodyA->GetPosition().y + sizeA.y > bodyB->GetPosition().y;
    bool bodyBIsAboveBodyA = bodyB->GetPosition().y + sizeB.y > bodyA->GetPosition().y;


    // Get the user data of the bodies (assuming they store pointers)
    UserData* userDataA = reinterpret_cast<UserData*>(bodyA->GetUserData().pointer);
    UserData* userDataB = reinterpret_cast<UserData*>(bodyB->GetUserData().pointer);


    if (userDataA && !userDataB) {
        flecs::entity entityA = ECS::getWorld().entity(userDataA->entity_id);



        if (entityA.has<PlayerInput>() && bodyBIsAboveBodyA) {

            entityA.get_mut<PlayerInput>()->isJumping = false;
        }



    }
    else if (!userDataA && userDataB) {
        flecs::entity entityB = ECS::getWorld().entity(userDataB->entity_id);

        if (entityB.has<PlayerInput>() && bodyAIsAboveBodyB) {
            entityB.get_mut<PlayerInput>()->isJumping = false;
        }

    }
    else if (userDataA && userDataB) {
        flecs::entity entityA = ECS::getWorld().entity(userDataA->entity_id);
        flecs::entity entityB = ECS::getWorld().entity(userDataB->entity_id);

        if (entityA.has<EnemyEntity>() && entityB.has<EnemyEntity>()) return;

        flecs::entity* player;
        flecs::entity* fireball;
        flecs::entity* enemy;

        bool doFireball = false;
        bool doPlayerHit = false;

        // Perform collision handling based on the entity types
        if (entityA.has<EnemyEntity>() && entityB.has<FireballEntity>()) {
            enemy = &entityA;
            fireball = &entityB;
            doFireball = true;
        }
        else if (entityA.has<FireballEntity>() && entityB.has<EnemyEntity>()) {
            fireball = &entityA;
            enemy = &entityB;
            doFireball = true;
        }
        else if (entityA.has<PlayerEntity>() && entityB.has<EnemyEntity>()) {
            player = &entityA;
            enemy = &entityB;
            doPlayerHit = true;
        }
        else if (entityA.has<EnemyEntity>() && entityB.has<PlayerEntity>()) {
            enemy = &entityA;
            player = &entityB;
            doPlayerHit = true;
        }


        if (doFireball && enemy) {
            flecs::entity owner = ECS::getWorld().entity(fireball->get<Owner>()->owner_id);

        	owner.get_mut<PlayerClass>()->doDamage(enemy, 1);

        	fireball->set<DeleteMe>({});

            enemy->get_mut<Health>()->currentHealth -= 10.f;
        }

        if (doPlayerHit && enemy) {
            player->get_mut<PlayerClass>()->playerHit(player);
        }
    } else
    {
	    
    }


}