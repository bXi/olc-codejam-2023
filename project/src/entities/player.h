#pragma once

#include "ecs.h"
#include "world/world.h"

#include "components/collision.h"
#include "components/playerindex.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"
#include "components/render2d.h"
#include "components/playerinput.h"


void CreatePlayerEntity(int index, vf2d pos);

