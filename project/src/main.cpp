#include "main.h"

int main(int argc, char* argv[])
{
	State::init();
	Settings::init();
	Input::init();

	SetTraceLogLevel(LOG_WARNING);

	const Image windowIcon = LoadImage("assets/raylib_512x512.png");

	SetWindowIcon(windowIcon);
	SetExitKey(KEY_NULL);

	ECS::init();

	State::setState(State::gamestate::GAME);

	// Main game loop
	while (State::getState() != State::gamestate::QUIT) {

		Audio::updateMusicStreams();
		Input::updateTimings();
		BeginDrawing();

		ClearBackground(BLACK);

		State::draw();

		if (WindowShouldClose()) State::setState(State::gamestate::QUIT);
		if (IsKeyPressed(KEY_F11)) Configuration::showGameStats = !Configuration::showGameStats;

		EndDrawing();
		Lerp::updateLerps();
	}

    State::unload();
	CloseAudioDevice();
	CloseWindow();        // Close window and OpenGL context

	return 0;
}

int WinMain(const int argc, char* argv[])
{
	return main(argc, argv);
}


