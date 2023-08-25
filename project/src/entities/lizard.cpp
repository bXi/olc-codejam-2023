#include "lizard.h"

void CreateLizardEntity(vf2d pos, float maxHealth) {

	const auto& ecs = ECS::getWorld();

	Texture2D sprite = Textures::getTexture("assets/monsters/lizard/lizard.png");

	flecs::entity entity;

	entity = ecs.entity()
		.set<Health>({ maxHealth, 32 })
		.set<EnemyEntity>({ })
		.set<AIController>({})
		.set<Sprite>({ 32.f, 64.f, sprite, false, false, 16.f, 48.f })
		.set<Render2D>({  });

	
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

	entity.emplace<RigidBody2D>(RigidBody, CircleShape.m_radius);

	entity.set<UserDataComponent>({ std::move(userData) });

	entity.get_mut<AIController>()->self = entity;

	
}
