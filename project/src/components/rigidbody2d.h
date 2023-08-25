#pragma once

#include <box2d/box2d.h>
#include "world/world.h"

struct UserData {
	flecs::entity_t entity_id;
};

struct UserDataComponent {
	std::unique_ptr<UserData> data;
};

struct RigidBody2D
{
	b2Body* RigidBody = nullptr;
	float radius;
};
