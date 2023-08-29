#include "player.h"



void CreatePlayerEntity(int index, vf2d pos) {

	const auto& ecs = ECS::getWorld();

	Texture2D sprite = Textures::getTexture("assets/players/idle.png");

	flecs::entity entity;
	// Create the player entity


	entity = ecs.entity()
		.set<PlayerEntity>({  })
		.set<PlayerIndex>({ index })
		.set<PlayerInput>({  })
		.set<PlayerClass>({  })
		.set<Sprite>({ 225.f, 376.f, sprite, true, false, 112.5f, 376.f, direction::EAST, 12, 1 })
		.set<Render2D>({})
		.emplace<Collision>(CATEGORY_FIREBALL, MASK_FIREBALL);
		





	b2Body* RigidBody = nullptr;

	b2CircleShape CircleShape;
	CircleShape.m_radius = 0.48f;

	auto userData = std::make_unique<UserData>();
	userData->entity_id = entity.id();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;

	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(userData.get());
	RigidBody = World::createBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &CircleShape;

	fixtureDef.density = 1.f;
	fixtureDef.friction = 0.0f;
	RigidBody->CreateFixture(&fixtureDef);



	entity.emplace<RigidBody2D>(RigidBody);


	entity.set<UserDataComponent>({ std::move(userData) });

	entity.get_mut<PlayerClass>()->init();
	entity.get_mut<Collision>()->init(&entity);
    entity.get_mut<Render2D>()->init(entity);

    entity.get_mut<PlayerInput>()->init(index);


}
