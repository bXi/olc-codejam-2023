#include "world.h"

#include "ecs.h"
#include "components/health.h"

#include "components/owner.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"

//PhysicsWall::PhysicsWall(Vector2 pos, Vector2 size)
//{
//    type = ObjectType::WALL;
//
//    ShapeColor = { 255, 0, 0, 72 };
//    Size = size;
//    HalfSize.x = size.x / 2;
//    HalfSize.y = size.y / 2;
//
//    b2BodyDef bodyDef;
//    bodyDef.type = b2_staticBody;
//    bodyDef.position.Set(pos.x, pos.y);
//    bodyDef.angle = 0.0f;
//    bodyDef.userData.pointer = 0;
//    RigidBody = World::createBody(&bodyDef);
//
//    Box.SetAsBox(Size.x / 2, Size.y / 2);
//
//    b2FixtureDef fixtureDef;
//    fixtureDef.shape = &Box;
//    fixtureDef.density = 1.0f;
//    fixtureDef.friction = 0.3f;
//
//    RigidBody->CreateFixture(&fixtureDef);
//}

void World::_init()
{

}

void World::_generateNewMap()
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