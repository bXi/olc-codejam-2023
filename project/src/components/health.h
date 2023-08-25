#pragma once
#include "ui/ui.h"

struct Health
{
	float maxHealth = -1;
	int healthBarYOffset = 0;


	float currentHealth = maxHealth;


	void draw(flecs::entity entity)
	{

		auto rigidBody2d = entity.get<RigidBody2D>();
		auto sprite = entity.get<Sprite>();



	}
};
