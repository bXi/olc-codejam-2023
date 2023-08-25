#pragma once

#include <flecs.h>

#include "components/rigidbody2d.h"


class ECS {
public:
	static void init()
	{
		get()._init();


	}

	static void reset()
	{
		get()._reset();
	}

	static flecs::world& getWorld()
	{
		return get()._ecs;
	}

	static void removeEntity(flecs::entity* entity)
	{

		if (entity->has<RigidBody2D>())
		{

			b2Body* body = entity->get_mut<RigidBody2D>()->RigidBody;

			// Delete the fixtures attached to the body
			b2Fixture* fixture = body->GetFixtureList();
			while (fixture) {
				b2Fixture* nextFixture = fixture->GetNext();
				body->DestroyFixture(fixture);
				fixture = nextFixture;
			}

			body->GetWorld()->DestroyBody(body);

		}

		entity->destruct();
	}


private:
	flecs::world _ecs;
	void _init()
	{
		//flecs::log::set_level(5);
		_ecs = flecs::world();
	}

	void _reset()
	{
		_ecs.reset();
		_ecs = flecs::world();

#ifdef DEBUG
		_ecs.set<flecs::Rest>({});
		_ecs.import<flecs::monitor>();
#endif
	}
public:
	ECS(const ECS&) = delete;
	static ECS& get() { static ECS instance; return instance; }

private:
	ECS() {}
};
