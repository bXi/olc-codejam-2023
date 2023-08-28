#pragma once

#include "raylib.h"

#include "flecs.h"


struct Render2D
{

	int i;


    void init(flecs::entity _entity){
        entity = _entity;
    }
    flecs::entity entity;

	void draw(flecs::entity* entity);

    Rectangle getTile(int tileId, bool doubleHeight = false);

    Rectangle getRectangle(int x, int y);
};
