#include "playerclass.h"

#include "rigidbody2d.h"


int PlayerClass::getRenderFrame(flecs::entity* entity)
{

    int spriteIndex = 0;

    float time = (GetTime() * 10.f);

    spriteIndex = (int)(time)%12;


    return spriteIndex;
}
