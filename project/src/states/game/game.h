#pragma once

#include <list>
#include <cstring>

//#include "optickglobal.h"
#include "raylib.h"

#include "utils/helpers.h"
#include "utils/lerp.h"

#include <input/inputhandler.h>
#include <fonts/fonthandler.h>
#include <log/loghandler.h>


//#include "entities/powerup.h"


#include "entities/player.h"
#include "entities/lizard.h"

#include "world/world.h"
#include "map/levelmanager.h"
#include "state/basestate.h"

inline auto doCirclesOverlap = [](float x1, float y1, float r1, float x2, float y2, float r2)
{
	return fabs((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) <= (r1 + r2) * (r1 + r2);
};

class GameState : public BaseState {
private:

	float accumulator = 0;
	float physTime = 1.f / static_cast<float>(Settings::getMonitorRefreshRate() * 2);
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	Camera2D camera = {0};

	Music backgroundMusic;


	float speed = 10.0f;

	float screenWidth;
	float screenHeight;

	float screenRatio;

    Rectangle cameraTargetArea;
    vf2d camTarget;

	LevelManager lm;
    LevelData leveldata;

    Vector2 positions[3];
    float speeds[3] = { .1f, .2f, .4f };
    float speedFactor = 2.f;
    int updateFrame = 0;

    Texture2D tilemap;
    Texture2D bg[3];

public:
	void load() override;
	void unload() override;
	void draw() override;

	void setupControls();

	void resetGame();

	void drawUI();

	void handleInput();
	void update();
	void drawEntities();

    Rectangle getRectangle(int _x, int _y)
	{
		const Rectangle rect = { static_cast<float>(_x * Configuration::tileWidth), static_cast<float>(_y * Configuration::tileHeight),  static_cast<float>(Configuration::tileWidth),  static_cast<float>(Configuration::tileHeight) };
		return rect;
	};

	Rectangle getTile(int tileId)
	{
		return getRectangle(tileId % 16, tileId / 16);
	};
};
