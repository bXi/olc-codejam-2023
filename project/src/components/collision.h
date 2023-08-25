#pragma once
#include <cstdint>

#include "rigidbody2d.h"

const uint16_t CATEGORY_FIREBALL = 0x0002;
const uint16_t MASK_FIREBALL = 0x0001;

struct Collision
{

	uint16_t category;
	uint16_t mask;


	Collision(uint16_t _category, uint16_t _mask) : category(_category), mask(_mask)
	{
		
	}

	void init(flecs::entity* entity)
	{
		auto* rigidBody = entity->get_mut<RigidBody2D>()->RigidBody;


		b2Filter fireballFilter;
		fireballFilter.categoryBits = category;
		fireballFilter.maskBits = mask;

		// Set the collision filtering for all fireball fixtures
		b2Fixture* fixture = rigidBody->GetFixtureList();
		while (fixture != nullptr) {
			fixture->SetFilterData(fireballFilter);
			fixture = fixture->GetNext();
		}
	}
	
};
