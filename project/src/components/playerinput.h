#pragma once
#include "input/inputhandler.h"

#include "utils/vectors.h"

#include "sprite.h"
#include "rigidbody2d.h"

struct PlayerInput
{
	std::vector<InputDevice*> controllers;

	vf2d vel = { 0.f, 0.f };
	vf2d aim = { 0.f, 0.f };

	int selectedWeapon = 1;
	bool shooting = false;
	bool isRunning = false;

    bool isJumping = false;
    bool startJumping = false;

	int weaponUpgrades[4] = { 0,0,0,0 };

    void init(int index)
    {

    }

	void addController(InputDevice* input)
	{
		controllers.emplace_back(input);
	}

	void doMovement(flecs::entity* entity)
	{
		isRunning = false;

		vel = { 0.f, 0.f };
		for (const auto& controller : controllers)
		{
			if (controller->is(Buttons::RUN, Action::HELD)) isRunning = true;

			if (controller->is(Buttons::ACCEPT, Action::PRESSED) ||
                controller->is(Buttons::UP, Action::PRESSED) ||
                controller->is(Buttons::SHOOT, Action::PRESSED)) {
                TraceLog(LOG_INFO, "Jump button pressed");
                if (!isJumping) {
                    isJumping = true;
                    startJumping = true;
                }
            }

			switch (controller->getType())
			{
			case InputType::MOUSE_KB:
				if (controller->is(Buttons::LEFT, Action::HELD)) vel.x -= 1.0f;
				if (controller->is(Buttons::RIGHT, Action::HELD)) vel.x += 1.0f;
				if (controller->is(Buttons::UP, Action::HELD)) vel.y -= 1.0f;
				if (controller->is(Buttons::DOWN, Action::HELD)) vel.y += 1.0f;
				
				break;
			case InputType::GAMEPAD:
				vel.x += GetGamepadAxisMovement(controller->getGamepadID(), GAMEPAD_AXIS_LEFT_X);
				//vel.y += GetGamepadAxisMovement(controller->getGamepadID(), GAMEPAD_AXIS_LEFT_Y);
				break;
			}
			if (vel.mag2() > 0)
			{
				//vel = vel.norm() * 128.f;
                vel *= 0.5f;
				if (entity->has<Sprite>())
				{
					auto sprite = entity->get_mut<Sprite>();
					if (vel.x > 0) 
						sprite->facing = direction::EAST;
					else if (vel.x < 0)
						sprite->facing = direction::WEST;

				}

			}
		}
	}

	void doShoot(flecs::entity entity, const Camera2D* camera)
	{

		shooting = false;
		auto* rigidBody2d = entity.get<RigidBody2D>();



		for (const auto& controller : controllers)
		{
			switch (controller->getType())
			{
			case InputType::MOUSE_KB:

				if (controller->is(Buttons::SHOOT, Action::HELD)) {
					const Vector2 mouseScreenPos = GetMousePosition();
					const Vector2 playerScreenPos = GetWorldToScreen2D({
						rigidBody2d->RigidBody->GetPosition().x * static_cast<float>(Configuration::tileWidth),
						rigidBody2d->RigidBody->GetPosition().y * static_cast<float>(Configuration::tileHeight)
						}, *camera);
					aim = {
						playerScreenPos.x - (mouseScreenPos.x + static_cast<float>(Configuration::tileWidth) * 0.5f),
						playerScreenPos.y - (mouseScreenPos.y + static_cast<float>(Configuration::tileHeight) * 0.5f)
					};
					aim *= -1.0f;
					shooting = true;
				}
				break;
			case InputType::GAMEPAD:
				vf2d _aim = { GetGamepadAxisMovement(controller->getGamepadID(), GAMEPAD_AXIS_RIGHT_X), GetGamepadAxisMovement(controller->getGamepadID(), GAMEPAD_AXIS_RIGHT_Y) };
				if (_aim.mag2() > 0) {
					aim = _aim;
					shooting = true;
				}
				break;
			}
		}
	}

};
