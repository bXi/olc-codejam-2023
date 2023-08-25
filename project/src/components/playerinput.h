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

	int weaponUpgrades[4] = { 0,0,0,0 };

    LerpAnimator* weaponSelectLerp = nullptr;
    LerpAnimator* weaponSelectVisibleLerp = nullptr;
    bool moveWeaponLeft = false;

    void init(int index)
    {
        weaponSelectLerp = Lerp::getLerp(TextFormat("weaponSelectLerpP%d", index), 0.0f, 1.0f, 0.4f);
        weaponSelectLerp->callback = EaseSineInOut;
        weaponSelectLerp->started = false;

        weaponSelectVisibleLerp = Lerp::getLerp(TextFormat("weaponSelectVisibleLerpP%d", index), 0.0f, 4.0f, 2.f);
        weaponSelectVisibleLerp->callback = EaseQuadIn;
        weaponSelectVisibleLerp->started = false;

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

			if (controller->is(Buttons::SWITCH_NEXT, Action::PRESSED)) {
                selectedWeapon++;
                weaponSelectLerp->time = 0.0f;
                weaponSelectLerp->started = true;
                moveWeaponLeft = false;
                weaponSelectVisibleLerp->time = 0.0f;
                weaponSelectVisibleLerp->started = false;

            }
			if (controller->is(Buttons::SWITCH_PREV, Action::PRESSED)) {
                selectedWeapon--;
                weaponSelectLerp->time = 0.0f;
                weaponSelectLerp->started = true;
                moveWeaponLeft = true;
                weaponSelectVisibleLerp->time = 0.0f;
                weaponSelectVisibleLerp->started = false;
            }

			if (selectedWeapon < 1) selectedWeapon = 4;
			if (selectedWeapon > 4) selectedWeapon = 1;

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
				vel.y += GetGamepadAxisMovement(controller->getGamepadID(), GAMEPAD_AXIS_LEFT_Y);
				break;
			}
			if (vel.mag2() > 0)
			{
				vel = vel.norm();
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
