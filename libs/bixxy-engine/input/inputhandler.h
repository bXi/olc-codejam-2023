#pragma once

#include <vector>
#include <list>
#include <map>
#include <unordered_map>

#include "config.h"
#include "raylib.h"

enum class InputType { GAMEPAD, MOUSE_KB };
enum class Buttons { LEFT, RIGHT, UP, DOWN, ACCEPT, BACK, SHOOT, SWITCH_NEXT, SWITCH_PREV, RUN };
enum class Action { HELD, PRESSED };

class InputDevice {
private:
    InputType type;
    int gamepadID;

    std::map<Buttons, std::list<int>> mappingKB = {
        { Buttons::LEFT,  { KEY_A }}, //, KEY_LEFT}},
        { Buttons::RIGHT, { KEY_D }}, //, KEY_RIGHT}},
        { Buttons::UP,    { KEY_W }}, //, KEY_UP}},
        { Buttons::DOWN,  { KEY_S }}, //, KEY_DOWN}},

        { Buttons::ACCEPT, { KEY_SPACE, KEY_ENTER, KEY_KP_ENTER }},
        { Buttons::BACK,   { KEY_ESCAPE, KEY_BACKSPACE }},

        { Buttons::SWITCH_NEXT, { KEY_TAB }},
        { Buttons::SWITCH_PREV, { KEY_GRAVE }},

        { Buttons::RUN, {KEY_LEFT_SHIFT}},

        { Buttons::SHOOT, { KEY_LEFT_ALT }}

    };

    std::map<Buttons, std::list<int>> mappingGP = {
        { Buttons::ACCEPT, { GAMEPAD_BUTTON_RIGHT_FACE_DOWN }},
        { Buttons::BACK,   { GAMEPAD_BUTTON_RIGHT_FACE_RIGHT }},

        { Buttons::LEFT,  { GAMEPAD_BUTTON_LEFT_FACE_LEFT }},
        { Buttons::RIGHT, { GAMEPAD_BUTTON_LEFT_FACE_RIGHT }},
        { Buttons::UP,    { GAMEPAD_BUTTON_LEFT_FACE_UP }},
        { Buttons::DOWN,  { GAMEPAD_BUTTON_LEFT_FACE_DOWN }},

        { Buttons::SWITCH_NEXT, { GAMEPAD_BUTTON_RIGHT_TRIGGER_1 }},
        { Buttons::SWITCH_PREV, { GAMEPAD_BUTTON_LEFT_TRIGGER_1 }},

        { Buttons::RUN, {GAMEPAD_BUTTON_LEFT_TRIGGER_2}},
    };


    bool isButtonPressed(Buttons button);
    bool isButtonHeld(Buttons button);

    std::map<Buttons, float> pressedTimings = {
        {Buttons::LEFT,  0.0f},
        {Buttons::RIGHT, 0.0f},
        {Buttons::UP,    0.0f},
        {Buttons::DOWN,  0.0f},
    };

    float joystickCooldown = 0.10f;


public:
    void updateTimings();

	bool is(Buttons button, Action action);

    int getGamepadID() const;

    InputDevice(InputType _type);

    InputDevice(InputType _type, int _gamepadID);
    InputType getType();
};


class Input {
public:

    static void init()
    {
        get()._init();
    }

    static InputDevice* getController(int index)
    {
        return get()._getController(index);
    }

    static void clear()
    {
        get()._clear();
    }

    static std::vector<InputDevice*> getAllInputs()
    {
        return get().inputs;
    }

    static void updateTimings()
    {
	    for (const auto&  i : get().inputs)
	    {
            i->updateTimings();
	    }
    }

    static bool keyPressed(int key)
    {
        return IsKeyPressed(key);
    }

private:
    std::vector<InputDevice*> inputs;
    void _init();
    InputDevice* _getController(int index);
    void _clear();
public:
    Input(const Input&) = delete;
    static Input& get() { static Input instance; return instance; }

private:
    Input() {}
};


