#pragma once

#include "raylib.h"

#include "flecs.h"


struct Render2D
{

	int i;

	void draw(flecs::entity* entity);
};
