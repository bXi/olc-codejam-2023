#include "ui.h"
#include "configuration/configuration.h"
#include "texture/texturehandler.h"
#include "log/loghandler.h"



void UI::drawDebugInfo() {
    constexpr int baseX = 30;
    constexpr int baseY = 270;
    int margin = 20;

    auto font = Fonts::getFont("assets/fonts/APL386.ttf", 20);

    int y = 0;


    Log::addLine("FPS:", TextFormat("%d", GetFPS()));

    Log::addLine("GameTime:", TextFormat("%.2f", Configuration::gameTime));
    Log::addLine("Bodies:", TextFormat("%d", World::getPhysicsWorld().GetBodyCount()));
    Log::addLine("SlowMoF:", TextFormat("%f", Configuration::slowMotionFactor));


    Log::addLine("");
    Log::addLine("-- Players --");

    const auto playerFilter = ECS::getWorld().filter<PlayerIndex, RigidBody2D>();
    playerFilter.each([&](flecs::entity entity, PlayerIndex playerIndex, RigidBody2D rigidBody2D) {
        Log::addLine(
                TextFormat("Player %d pos:", playerIndex.index),
                TextFormat("X: %.2f Y: %.2f", rigidBody2D.RigidBody->GetPosition().x,
                           rigidBody2D.RigidBody->GetPosition().y)
        );

        Log::addLine(
                TextFormat("Player %d vel:", playerIndex.index),
                TextFormat("X: %.2f Y: %.2f", rigidBody2D.RigidBody->GetLinearVelocity().x,
                           rigidBody2D.RigidBody->GetLinearVelocity().y)
        );
    });

    auto lines = Log::getLines();

    Rectangle backgroundRect = {
            static_cast<float>(baseX - margin),
            static_cast<float>(baseY - margin),
            static_cast<float>(Log::getHeaderWidth() + Log::getLongestLineWidth() + (margin * 3)),
            static_cast<float>((lines.size() * 25) + (margin * 2) - 10)
    };
    DrawRectangleRounded(backgroundRect, .1f, 32, {0, 0, 0, 160});


    for (const auto &line: lines) {
        DrawTextEx(font, line.first.c_str(), vi2d(baseX, baseY + y), 20.f, 0.0f, LIME);
        DrawTextEx(font, line.second.c_str(), vi2d(baseX + Log::getHeaderWidth() + margin, baseY + y), 20.f, 0.0f, LIME);
        y += 25;
    }


}
