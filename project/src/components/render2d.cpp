#include "render2d.h"

#include "rigidbody2d.h"
#include "sprite.h"
#include "playerclass.h"
#include "configuration/configuration.h"
#include "texture/texturehandler.h"
#include "utils/vectors.h"

void Render2D::draw(flecs::entity* entity)
{

	if (entity->has<Sprite>()) {

		auto* sprite = entity->get<Sprite>();
		auto* rigidBody = entity->get_mut<RigidBody2D>();

		int renderFrame = 0;



		if (entity->has<PlayerClass>())
		{
			auto* playerClass = entity->get_mut<PlayerClass>();

			renderFrame = playerClass->getRenderFrame(entity);

		}


		const vf2d pos = rigidBody->RigidBody->GetPosition();

		const Rectangle position = {
			pos.x * static_cast<float>(Configuration::tileWidth) - sprite->originX,
			pos.y * static_cast<float>(Configuration::tileHeight) - sprite->originY,
			sprite->width,
			sprite->height
		};

		Rectangle source;
		if (sprite->multiSheet) {
			source = Textures::getTile(renderFrame, sprite->doubleHeight);
		}
		else
		{
			source = { 0.f, 0.f, sprite->width, sprite->height };
		}

		if (sprite->facing == direction::WEST) {
			source.width *= -1.0f;
		}

		DrawTexturePro(sprite->sprite, source, position, { 0,0 }, 0.0f, WHITE);
	}
}
