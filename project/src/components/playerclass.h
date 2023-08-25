#pragma once

#include "ecs.h"
#include "raylib.h"
#include "rigidbody2d.h"

#include "world/world.h"



struct PlayerClass
{
	void init()
	{

	}


	void doDamage(flecs::entity* entity, int weaponId)
	{

	}

	int getSpriteIndex() const
	{
        return 0;
	}

	void shoot(flecs::entity entity)
	{

	}

	void update()
	{

	}

	int getRenderFrame(flecs::entity* entity);


	float hitTimer = 0.0f;
	void playerHit(flecs::entity* entity)
	{


	}

};