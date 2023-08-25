#pragma once

#include "raylib.h"
#include <algorithm>
#include "configuration/configuration.h"

#include "utils/lerp.h"

#include "state/basestate.h"

class IntroState : public BaseState {
	Texture2D logo_raylib;
	float grey = 0.0f;
	float intro_logo_time = 0.2f;
	float total_intro_time = 4.0f;

	float gameTime = 0.0f;

	bool isLoaded = false;


	LerpAnimator* firstLogoLerp = nullptr;
	public:
		
		void load() override;
		void unload() override;
		void draw() override;
};
