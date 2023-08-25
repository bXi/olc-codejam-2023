#pragma once

#include "ecs.h"
#include "world/world.h"

#include "components/collision.h"
#include "components/playerindex.h"
#include "components/rigidbody2d.h"
#include "components/playerclass.h"
#include "components/render2d.h"
#include "components/sprite.h"
#include "components/health.h"
#include "components/aicontroller.h"


void CreateLizardEntity(vf2d pos, float maxHealth);

