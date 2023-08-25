#include "player.h"



void CreatePlayerEntity(int index, vf2d pos) {

	const auto& ecs = ECS::getWorld();

	Texture2D sprite = Textures::getTexture("assets/entities/entities.png");

	flecs::entity entity;
	// Create the player entity


	entity = ecs.entity()
		.set<PlayerEntity>({  })
		.set<PlayerIndex>({ index })
		.set<PlayerInput>({  })
		.set<PlayerClass>({  })
		.set<Sprite>({ 32.f, 56.f, sprite, true, true, 16.f, 40.f, direction::WEST })
		.set<Render2D>({})
		.emplace<Collision>(CATEGORY_FIREBALL, MASK_FIREBALL);
		





	b2Body* RigidBody = nullptr;

	b2CircleShape CircleShape;
	CircleShape.m_radius = 0.5f;

	auto userData = std::make_unique<UserData>();
	userData->entity_id = entity.id();

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(pos.x, pos.y);
	bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(userData.get());
	RigidBody = World::createBody(&bodyDef);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &CircleShape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	RigidBody->CreateFixture(&fixtureDef);
	
	entity.emplace<RigidBody2D>(RigidBody);


	entity.set<UserDataComponent>({ std::move(userData) });

	entity.get_mut<PlayerClass>()->init();
	entity.get_mut<Collision>()->init(&entity);

    entity.get_mut<PlayerInput>()->init(index);
	
}
