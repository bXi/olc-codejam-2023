#include "intro.h"

#include "state/state.h"

void IntroState::load()
{
	logo_raylib = LoadTexture("assets/raylib_512x512.png");
	firstLogoLerp = Lerp::getLerp("firstLogoLerp", 0.0f, 255.0f, intro_logo_time);

	isLoaded = true;
}

void IntroState::unload()
{
	if (isLoaded) {
		Lerp::resetTime("firstLogoLerp");
		UnloadTexture(logo_raylib);
		gameTime = 0.0f;
	}
	isLoaded = false;
}


void IntroState::draw()
{

	if (!isLoaded) load();
	gameTime += static_cast<float>(GetFrameTime());

	grey = std::min(firstLogoLerp->getValue(), 255.0f);

	

	if (gameTime > total_intro_time) {
		unload();
		State::setState(State::gamestate::GAME);
	}


	const auto _g = static_cast<unsigned char>(grey);
	const Color intro_background_color = {_g, _g, _g};
	const Color tint_logo = { _g, _g , _g , _g };
	ClearBackground(intro_background_color);
	DrawTexture(logo_raylib, GetScreenWidth() / 2 - logo_raylib.width / 2, GetScreenHeight() / 2 - logo_raylib.height / 2, tint_logo);

	//DrawText(TextFormat("Time: %f", static_cast<double>(grey)), 20, 20, 16, RED);
}