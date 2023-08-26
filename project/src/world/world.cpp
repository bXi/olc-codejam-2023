#include "world.h"

#include "ecs.h"
#include "components/health.h"

#include "components/owner.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"

void World::_loadLevelPhysics(LevelData ld)
{
    SetTraceLogLevel(LOG_INFO);

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

            float offSetX = (float) x * ((float) Configuration::tileWidth / levelScale);
            float offSetY = (float) y * ((float) Configuration::tileHeight / levelScale);

            Shape* shape = uniqueShapes[tileId - 1];

            if (shape->type == Shape::RECTANGLE) {

                wRectangle* rect = (wRectangle*)shape;

                PhysicsRectangle* r = new PhysicsRectangle();

                r->ShapeColor = Color{230, 41, 55, 127};

                r->Size.x = rect->width / levelScale;
                r->Size.y = rect->height / levelScale;

                r->HalfSize.x = (rect->width / 2) / levelScale;
                r->HalfSize.y = (rect->height / 2) / levelScale;

                b2BodyDef bodyDef;
                bodyDef.type = b2_staticBody;
                bodyDef.position.Set((offSetX + (rect->x / levelScale)) / ((float)Configuration::tileWidth / levelScale), (offSetY + (rect->y / levelScale))/ ((float)Configuration::tileWidth / levelScale));
                bodyDef.angle = 0.0f;
                bodyDef.userData.pointer = 0;
                r->RigidBody = World::createBody(&bodyDef);

                b2PolygonShape Box;
                Box.SetAsBox((r->Size.x / 2) / levelScale, (r->Size.y / 2) / levelScale);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &Box;
                fixtureDef.density = 1.0f;
                fixtureDef.friction = 0.3f;

                r->RigidBody->CreateFixture(&fixtureDef);

                addObject(r);
            }



        }
    }

    SetTraceLogLevel(LOG_WARNING);
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

    // Get the user data of the bodies (assuming they store pointers)
    UserData* userDataA = reinterpret_cast<UserData*>(bodyA->GetUserData().pointer);
    UserData* userDataB = reinterpret_cast<UserData*>(bodyB->GetUserData().pointer);


    if (userDataA && !userDataB) {
        flecs::entity entityA = ECS::getWorld().entity(userDataA->entity_id);
        if (entityA.has<FireballEntity>() && bodyB->GetType() == b2_staticBody) {
            entityA.set<DeleteMe>({});
        }
    }
    else if (!userDataA && userDataB) {
        flecs::entity entityB = ECS::getWorld().entity(userDataB->entity_id);

        if (
            entityB.has<FireballEntity>() &&
            bodyA->GetType() == b2_staticBody)
        {
            entityB.set<DeleteMe>({});
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