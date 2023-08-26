#pragma once

#include <mutex>
#include <thread>

#include "raylib.h"

#include "configuration/configuration.h"
#include "texture/texturehandler.h"

#include <nlohmann/json.hpp>
#include <fstream>

struct LevelData
{
    int height;
    int width;

    std::vector<int> floorTiles;
    std::vector<int> decorTiles;

};


class LevelManager
{
private:

public:

    LevelData currentlevel;

	LevelManager();

    void loadLevel(const char* levelnumber);



};


