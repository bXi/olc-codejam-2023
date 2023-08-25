#include "inputhandler.h"

bool InputDevice::isButtonPressed(Buttons button)
{
	int keysHeld = 0;
	switch (type) {
	case InputType::GAMEPAD:
		if (button == Buttons::LEFT && pressedTimings[Buttons::LEFT] == 0.0f && GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) != 0.0f) {
			keysHeld += (GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) < 0.0f) ? 1 : 0;
			pressedTimings[Buttons::LEFT] = joystickCooldown;
		}
		if (button == Buttons::RIGHT && pressedTimings[Buttons::RIGHT] == 0.0f && GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) != 0.0f) {
			keysHeld += (GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X) > 0.0f) ? 1 : 0;
			pressedTimings[Buttons::RIGHT] = joystickCooldown;
		}
		if (button == Buttons::UP && pressedTimings[Buttons::UP] == 0.0f && GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) != 0.0f) {
			keysHeld += (GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) < 0.0f) ? 1 : 0;
			pressedTimings[Buttons::UP] = joystickCooldown;
		}
		if (button == Buttons::DOWN && pressedTimings[Buttons::DOWN] == 0.0f) {
			keysHeld += (GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y && GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_Y) != 0.0f) > 0.0f) ? 1 : 0;
			pressedTimings[Buttons::DOWN] = joystickCooldown;
		}

		for (const auto& key : mappingGP[button])
			keysHeld += static_cast<int>(IsGamepadButtonPressed(gamepadID, key));
		return (keysHeld != 0);
	case InputType::MOUSE_KB:

		for (const auto& key : mappingKB[button])
			keysHeld += static_cast<int>(Input::keyPressed(key));
		return (keysHeld != 0);
	}
	return false;
}

bool InputDevice::isButtonHeld(Buttons button)
{
	int keysHeld = 0;
	switch (type) {
	case InputType::GAMEPAD:
		for (const auto& key : mappingGP[button])
			keysHeld += static_cast<int>(IsGamepadButtonDown(gamepadID, key));
		return (keysHeld != 0);
	case InputType::MOUSE_KB:
		if (button == Buttons::SHOOT)
		{
			keysHeld += static_cast<int>(IsMouseButtonDown(MOUSE_BUTTON_LEFT));

		}
		for (const auto& key : mappingKB[button])
			keysHeld += static_cast<int>(IsKeyDown(key));
		return (keysHeld != 0);
	}
	return false;
}


bool InputDevice::is(Buttons button, Action action) {
	switch (action) {
	case Action::HELD:    return isButtonHeld(button);
	case Action::PRESSED: return isButtonPressed(button);
	}
	return false;
}

int InputDevice::getGamepadID() const
{
	return gamepadID;
}

InputDevice::InputDevice(InputType _type) {
	type = _type;
	gamepadID = -1;
}

InputDevice::InputDevice(InputType _type, int _gamepadID)
{
	type = _type;
	gamepadID = _gamepadID;
}

InputType InputDevice::getType()
{
	return type;
}

void InputDevice::updateTimings()
{
	for (auto& timing : pressedTimings)
	{
		if (timing.second > 0.0f)
		{
			timing.second -= GetFrameTime();
		}
		else
		{
			timing.second = 0.0f;
		}
	}
}



void Input::_init()
{
	inputs.push_back(new InputDevice(InputType::MOUSE_KB));

	for (int i = 0; i < MAX_GAMEPADS; i++) {
		if (IsGamepadAvailable(i)) {
			//std::cout << "W&L: adding gamepad: " << i << std::endl;
			inputs.push_back(new InputDevice(InputType::GAMEPAD, i));
		}
	}
}

InputDevice* Input::_getController(int index)
{
	return inputs[index];
}

void Input::_clear()
{
	inputs.clear();
}

