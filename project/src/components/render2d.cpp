#include "render2d.h"

#include "rigidbody2d.h"
#include "sprite.h"
#include "playerclass.h"
#include "configuration/configuration.h"
#include "texture/texturehandler.h"
#include "utils/vectors.h"

void Render2D::draw(flecs::entity* _entity)
{

	if (_entity->has<Sprite>()) {

		auto* sprite = _entity->get<Sprite>();
		auto* rigidBody = _entity->get_mut<RigidBody2D>();

		int renderFrame = 0;



		if (_entity->has<PlayerClass>())
		{
			auto* playerClass = _entity->get_mut<PlayerClass>();

			renderFrame = playerClass->getRenderFrame(_entity);

		}


		const vf2d pos = rigidBody->RigidBody->GetPosition();

		const Rectangle position = {
            (pos.x * 64.f) + 32.f,
            (pos.y * 64.f) + 16.f,
			sprite->width / 4.f,
			sprite->height / 4.f
		};

		Rectangle source;

#ifndef EMSCRIPTEN
		if (sprite->multiSheet) {
			source = getTile(renderFrame, sprite->doubleHeight);
		}
		else
		{
			source = { 0.f, 0.f, sprite->width, sprite->height };
		}
#else
        source = { 0.f, 0.f, sprite->width, sprite->height };
#endif

		if (sprite->facing == direction::WEST) {
			source.width *= -1.0f;
		}



		DrawTexturePro(sprite->sprite, source, position, {position.width / 2.f,position.height / 2.f }, 0.0f, WHITE);

        //Color a = {255,255,255,54};
        //DrawCircle(position.x , position.y , 48.f, a);
        //DrawCircle(position.x + 32.f , position.y + 32.f , 32.f, RED);
        //DrawCircle(position.x, position.y + 16.f, 32.f, {255,0,0,40});
	}
}

Rectangle Render2D::getTile(int tileId, bool doubleHeight)
{

    auto* sprite = entity.get_mut<Sprite>();

    Rectangle r = getRectangle(tileId % sprite->spritesWidth, (int)tileId / sprite->spritesHeight);
    if (doubleHeight) {
        r.y -= r.height;
        r.height *= 2;
    }
    return r;
}

Rectangle Render2D::getRectangle(int x, int y)
{
    auto* sprite = entity.get_mut<Sprite>();

    const Rectangle rect = { static_cast<float>(x * sprite->width ), static_cast<float>(y * sprite->height),  static_cast<float>(sprite->width),  static_cast<float>(sprite->height) };
    return rect;
};