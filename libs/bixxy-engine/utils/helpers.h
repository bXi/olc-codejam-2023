#pragma once

#include "raylib.h"
#include "configuration/configuration.h"
#include "utils/vectors.h"

class Helpers {
public:
	static int clamp(int input, int min, int max);
	static float mapValues(float x, float in_min, float in_max, float out_min, float out_max);
	static void DrawMainMenu();
	static float getDifficultyModifier(float mod);

	static bool lineIntersectsRectangle(vf2d linestart, vf2d lineend, Rectangle rect);

	static std::vector<std::pair<vf2d, vf2d>> getLinesFromRectangle(Rectangle rect);

	static bool randomChance(const float required);

};
