#pragma once

#include "ecs.h"
#include "world/world.h"

#include "components/collision.h"
#include "components/playerindex.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"
#include "components/render2d.h"
#include "components/playerinput.h"
#include "components/type.h"
#include "components/owner.h"

void CreateFireballEntity(flecs::entity* player, vf2d pos, vf2d aim);

