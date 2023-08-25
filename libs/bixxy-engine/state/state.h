#pragma once

#include <random>
#include <iostream>

#include "raylib.h"

#include "audio/audiohandler.h"

#include "basestate.h"


#include "states/intro/intro.h"
#include "states/game/game.h"


class State {
public:
	enum class gamestate {
		INTRO,
		GAME,
		QUIT,
	};

    static void draw()
    {
        get()._draw();
    }

	static void init()
	{
		get()._init();
	}

    static void unload()
    {
        get()._unload();
    }

	static void setState(gamestate newstate)
	{
		get()._setState(newstate);
	}

	static gamestate getState()
	{
		return get()._getState();
	}

private:

    BaseState* state;

	gamestate currentstate = gamestate::INTRO;

	void _setState(gamestate newstate)
	{

		if (newstate != currentstate) {
            state->unload();

			currentstate = newstate;
			Audio::stopMusic();

            switch (currentstate) {
                case State::gamestate::INTRO:
                    state = new IntroState;
                    break;
                case State::gamestate::GAME:
                    state = new GameState;
                    break;
            }

            state->load();
		}

	}
	gamestate _getState() { return currentstate; }

    void _draw() {
        state->draw();
    }

    void _unload() {
        state->unload();
    }


	void _init()
    {
        state = new IntroState;
		currentstate = gamestate::INTRO;
	}
public:
	State(const State&) = delete;
	static State& get() { static State instance; return instance; }

private:
	State() {}
};
