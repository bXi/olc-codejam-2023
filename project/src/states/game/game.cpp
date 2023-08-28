#include "game.h"
#include "state/state.h"

void GameState::setupControls() {

    const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
    playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
        auto *input = entity.get_mut<PlayerInput>();

        input->controllers.clear();

        if (index.index == 1) {
            input->addController(Input::getController(0));

            if (Input::getAllInputs().size() > 1) {
                if (playerFilter.count() == 1)
                    input->addController(Input::getController(1));
            }
        }
    });
}

void GameState::resetGame() {
    ECS::reset();

    lm.loadLevel("01");
    leveldata = lm.currentlevel;

    cameraTargetArea = {
        640.f,
        180.f,
        ((leveldata.width * Configuration::tileWidth) / 2.f) - (640.f * 2.f),
        200.f,
    };

    World::loadLevelPhysics(leveldata);


//    CreatePlayerEntity(1, )



    setupControls();

    vf2d camPos = {0.0f, 0.0f};

    const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
    playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
        auto *rigidBody = entity.get<RigidBody2D>()->RigidBody;

        //rigidBody->SetTransform(World::getStart(), rigidBody->GetAngle());
        camPos += rigidBody->GetPosition();
    });

    camera.target = {(camPos.x / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileWidth),
                     (camPos.y / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileHeight)};

    accumulator += GetFrameTime();
    while (accumulator >= physTime) {
        accumulator -= physTime;
        World::doStep(physTime, velocityIterations, positionIterations);
    }
}

void GameState::load() {
    Fonts::getFont("assets/fonts/APL386.ttf", 20);

    tilemap = Textures::getTexture("assets/tilesets/tiles.png");
    bg[0] = Textures::getTexture("assets/bg/bg0.png");
    bg[1] = Textures::getTexture("assets/bg/bg1.png");
    bg[2] = Textures::getTexture("assets/bg/bg2.png");

    for (int i = 0; i < 3; ++i) {
        positions[i] = (Vector2) {0.f, -(2.f - (float) i) * 65.f};
    }


    screenWidth = static_cast<float>(GetScreenWidth());
    screenHeight = static_cast<float>(GetScreenHeight());
    screenRatio = screenWidth / screenHeight;

    camera.offset = {static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    camTarget = {640.f, 380.f};

    resetGame();


}

void GameState::unload() {}

void GameState::draw() {

    ClearBackground(World::getBackgroundColor());


    vi2d target = vi2d(camera.target) / Configuration::tileWidth;
    vf2d camPos = {0.0f, 0.0f};

    const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
    playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
        auto *rigidBody2d = entity.get<RigidBody2D>();
        auto *input = entity.get_mut<PlayerInput>();
        auto *playerClass = entity.get_mut<PlayerClass>();

        input->doMovement(&entity);
        input->doShoot(entity, &camera);

        playerClass->update();

        auto newSpeed = speed;
        if (input->isRunning) newSpeed *= 1.2f;

        newSpeed /= Configuration::slowMotionFactor;

        rigidBody2d->RigidBody->SetLinearVelocity(input->vel * newSpeed);

        camPos += rigidBody2d->RigidBody->GetPosition();
    });


    vf2d averagePlayerPos = {(camPos.x / static_cast<float>(playerFilter.count())), // * static_cast<float>(Configuration::tileWidth),
                             (camPos.y / static_cast<float>(playerFilter.count())) // * static_cast<float>(Configuration::tileHeight)
    };

    handleInput();
    update();


//    camTarget = {(camPos.x / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileWidth),
//                      (camPos.y / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileHeight)};






    camera.target = camTarget.clamp(cameraTargetArea);

    const auto aiFilter = ECS::getWorld().filter<AIController>();
    aiFilter.each([&](flecs::entity entity, AIController aicontroller) {
        aicontroller.seek();
    });


    accumulator += GetFrameTime();
    while (accumulator >= physTime) {
        accumulator -= physTime;
        World::doStep(physTime, velocityIterations, positionIterations);
    }


    for (int i = 0; i < 3; ++i) {

        positions[i].x -= speeds[i] * speedFactor;

        if (positions[i].x <= -((bg[i].width / 3) + 200.f)) {
            positions[i].x += (bg[i].width / 3);
        }

    }

    for (int i = 0; i < 3; ++i) {
        DrawTextureEx(bg[i], positions[i], 0.0f, 1.f, WHITE);
    }


    BeginMode2D(camera);

    for (int x = 0; x < leveldata.width; x++) {
        for (int y = 0; y < leveldata.height; y++) {
            int index = y * leveldata.width + x;

            Rectangle dest = {
                    (float) x * ((float) Configuration::tileWidth / World::getLevelScale()),
                    (float) y * ((float) Configuration::tileHeight / World::getLevelScale()),
                    128.f / World::getLevelScale(),
                    128.f / World::getLevelScale(),

            };

            DrawTexturePro(tilemap, getTile(leveldata.floorTiles.at(index) - 1), dest, {0.f, 0.f}, 0.f, WHITE);
            DrawTexturePro(tilemap, getTile(leveldata.decorTiles.at(index) - 1), dest, {0.f, 0.f}, 0.f, WHITE);
        }
    }


    playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
        auto *input = entity.get_mut<PlayerInput>();
        auto *playerClass = entity.get_mut<PlayerClass>();

        if (input->shooting) {
            playerClass->shoot(entity);
        }
    });
    drawEntities();

    World::draw();


    EndMode2D();

    drawUI();

    const auto deleteFilter = ECS::getWorld().filter<DeleteMe>();
    std::vector<flecs::entity> entitiesToDelete;
    deleteFilter.each([&](flecs::entity entity, DeleteMe _dm) {
        entitiesToDelete.push_back(entity);

    });

    for (auto &entity: entitiesToDelete) ECS::removeEntity(&entity);

    Configuration::gameTime += GetFrameTime();
    ECS::getWorld().progress(GetFrameTime());
}

void GameState::drawEntities() {
    std::vector<flecs::entity> renderableEntities;

    const auto renderFilter = ECS::getWorld().filter<Render2D>();
    renderFilter.each([&](flecs::entity entity, Render2D renderer) {
        renderableEntities.push_back(entity);
        renderer.draw(&entity);
    });

    std::sort(renderableEntities.begin(), renderableEntities.end(),
              [](const flecs::entity &entity1, const flecs::entity &entity2) {
                  const vf2d pos1 = entity1.get<RigidBody2D>()->RigidBody->GetPosition();
                  const vf2d pos2 = entity2.get<RigidBody2D>()->RigidBody->GetPosition();
                  return pos1.y < pos2.y;
              });

    for (auto &entity: renderableEntities) {
        entity.get_mut<Render2D>()->draw(&entity);

    }

    std::vector<flecs::entity> entitiesHealth;

//    const auto healthFilter = ECS::getWorld().filter<Health>();
//    healthFilter.each([&](flecs::entity entity, Health health) {
//
//        health.draw(entity);
//
//        if (health.currentHealth <= 0.0f) {
//            entitiesHealth.push_back(entity);
//
//        }
//    });

    for (auto entity: entitiesHealth) {
        entity.set<DeleteMe>({});
    }

}

void GameState::update() {

}

void GameState::handleInput() {
    //only for global input
    if (IsKeyPressed(KEY_LEFT)) camTarget.x -= (IsKeyDown(KEY_LEFT_SHIFT)) ? 1.f : 100.f;
    if (IsKeyPressed(KEY_RIGHT)) camTarget.x +=  (IsKeyDown(KEY_LEFT_SHIFT)) ? 1.f : 100.f;
    if (IsKeyPressed(KEY_UP)) camTarget.y -=  (IsKeyDown(KEY_LEFT_SHIFT)) ? 1.f : 100.f;
    if (IsKeyPressed(KEY_DOWN)) camTarget.y +=  (IsKeyDown(KEY_LEFT_SHIFT)) ? 1.f : 100.f;



    Log::addLine("CamTarget:", camTarget.str());

}

void GameState::drawUI() {
    // UI
    if (Configuration::showGameStats) {
        vf2d mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        mousePos /= (Configuration::tileWidth / World::getLevelScale());
        Log::addLine("Mousepos", mousePos.str());
        UI::drawDebugInfo();
    }
    Log::resetLog();

    if (IsKeyPressed(KEY_N)) {


        resetGame();

    }


}
