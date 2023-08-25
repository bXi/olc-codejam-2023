#include "fireball.h"

void CreateFireballEntity(flecs::entity* player, vf2d pos, vf2d vel) {

	const auto& ecs = ECS::getWorld();

	Texture2D sprite = Textures::getTexture("assets/players/wizard/fireball.png");

	flecs::entity entity;

	entity = ecs.entity()
		.set<FireballEntity>({ })
		.set<Sprite>({ 8.f, 8.f, sprite })
		.set<Render2D>({  });

	b2Body* RigidBody = nullptr;

	b2CircleShape CircleShape;
	CircleShape.m_radius = 0.125f;

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

	entity.emplace<Collision>(CATEGORY_FIREBALL, MASK_FIREBALL);

	entity.get_mut<Collision>()->init(&entity);

	entity.emplace<Owner>(player->id());

	vf2d velocity = vel.norm() * 20;
	entity.get_mut<RigidBody2D>()->RigidBody->SetLinearVelocity(velocity);
}
