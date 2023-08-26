#include "game.h"
#include "state/state.h"

void GameState::setupControls()
{

	const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
	playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
		auto* input = entity.get_mut<PlayerInput>();

		input->controllers.clear();

		if (index.index == 1)
		{
			input->addController(Input::getController(0));

			if (Input::getAllInputs().size() > 1)
			{
				if (playerFilter.count() == 1)
					input->addController(Input::getController(1));
			}
		}
	});
}

void GameState::resetGame()
{
	ECS::reset();


	setupControls();

	vf2d camPos = { 0.0f, 0.0f };

	const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
	playerFilter.each([&](flecs::entity entity, PlayerIndex index){
		auto* rigidBody = entity.get<RigidBody2D>()->RigidBody;

		//rigidBody->SetTransform(World::getStart(), rigidBody->GetAngle());
		camPos += rigidBody->GetPosition();
	});

	camera.target = { (camPos.x / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileWidth), (camPos.y / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileHeight) };

	accumulator += GetFrameTime();
	while (accumulator >= physTime)
	{
		accumulator -= physTime;
		World::doStep(physTime, velocityIterations, positionIterations);
	}
}

void GameState::load()
{
    Fonts::getFont("assets/fonts/APL386.ttf", 20);

    tilemap = Textures::getTexture("assets/tilesets/tiles.png");
    bg[0] = Textures::getTexture("assets/bg/bg0.png");
    bg[1] = Textures::getTexture("assets/bg/bg1.png");
    bg[2] = Textures::getTexture("assets/bg/bg2.png");

    for (int i = 0; i < 3; ++i) {
        positions[i] = (Vector2){ 0.f, -(2.f - (float)i) * 65.f };
    }


	screenWidth = static_cast<float>(GetScreenWidth());
	screenHeight = static_cast<float>(GetScreenHeight());
	screenRatio = screenWidth / screenHeight;

	camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	resetGame();


}

void GameState::unload()
{}

void GameState::draw()
{

	ClearBackground(World::getBackgroundColor());


	vi2d target = vi2d(camera.target) / Configuration::tileWidth;
	vf2d camPos = { 0.0f, 0.0f };

    const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
	playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
		auto* rigidBody2d = entity.get<RigidBody2D>();
		auto* input = entity.get_mut<PlayerInput>();
		auto* playerClass = entity.get_mut<PlayerClass>();

		input->doMovement(&entity);
		input->doShoot(entity, &camera);

		playerClass->update();

		auto newSpeed = speed;
		if (input->isRunning) newSpeed *= 1.2f;

		newSpeed /= Configuration::slowMotionFactor;

		rigidBody2d->RigidBody->SetLinearVelocity(input->vel * newSpeed);

		camPos += rigidBody2d->RigidBody->GetPosition();
	});


    vf2d averagePlayerPos = { (camPos.x / static_cast<float>(playerFilter.count())), // * static_cast<float>(Configuration::tileWidth),
                              (camPos.y / static_cast<float>(playerFilter.count())) // * static_cast<float>(Configuration::tileHeight)
                       };

	handleInput();
	update();





    vf2d camTarget = { (camPos.x / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileWidth),
                       (camPos.y / static_cast<float>(playerFilter.count())) * static_cast<float>(Configuration::tileHeight) };


	camera.target = camTarget;

	const auto aiFilter = ECS::getWorld().filter<AIController>();
	aiFilter.each([&](flecs::entity entity, AIController aicontroller) {
		aicontroller.seek();
	});


	accumulator += GetFrameTime();
	while (accumulator >= physTime)
	{
		accumulator -= physTime;
		World::doStep(physTime, velocityIterations, positionIterations);
	}


     for (int i = 0; i < 3; ++i) {

        positions[i].x -= speeds[i] * speedFactor;

        if (positions[i].x <= -((bg[i].width / 3) + 200.f))
        {
            positions[i].x += (bg[i].width / 3);
        }

     }

     for (int i = 0; i < 3; ++i) {
         DrawTextureEx(bg[i], positions[i], 0.0f, 1.f, WHITE);
     }

	BeginMode2D(camera);

//    widthMargin = 1.f + ((GetScreenWidth() / Configuration::tileWidth) / 2.f) / camera.zoom;
//    heightMargin = 1.f + ((GetScreenHeight() / Configuration::tileHeight) / 2.f) / camera.zoom;

//    int leftMargin = std::max(static_cast<int>(camera.target.x) / Configuration::tileWidth - widthMargin, 0);
//    int rightMargin = std::min(leftMargin + widthMargin * 2 + 1, tileData.width);
//
//    int topMargin = std::max(static_cast<int>(camera.target.y) / Configuration::tileHeight - heightMargin, 0);
//    int bottomMargin = std::min(topMargin + heightMargin * 2 + 1, tileData.height);

//
//	for (int y = topMargin; y < bottomMargin; y++) {
//		for (int x = leftMargin; x < rightMargin; x++) {
//            int index = y * tileData.width + x;
//
//            const Rectangle destRect = { static_cast<float>(x * Configuration::tileWidth), static_cast<float>(y * Configuration::tileHeight),  static_cast<float>(Configuration::tileWidth),  static_cast<float>(Configuration::tileHeight) };
//            const Rectangle srcRect = getTile(tileData.tiles.at(index));
//
//            DrawTexturePro(dungeonTileset, srcRect, destRect, vf2d(0.f, 0.f), 0.f,WHITE);
//
//
//        }
//    }

	playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
		auto* input = entity.get_mut<PlayerInput>();
		auto* playerClass = entity.get_mut<PlayerClass>();

		if (input->shooting)
		{
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
	
	for (auto& entity: entitiesToDelete) ECS::removeEntity(&entity);

	if (World::getSlowMotionLerp()->started)
	{
		Configuration::slowMotionFactor = World::getSlowMotionLerp()->getValue();
	}

	if (World::getSlowMotionLerp()->isFinished())
	{
		World::getSlowMotionLerp()->started = false;
	}

	World::setSlowMotionTimer(World::getSlowMotionTimer() - GetFrameTime()); // / Configuration::slowMotionFactor
	Configuration::gameTime += GetFrameTime();
	ECS::getWorld().progress(GetFrameTime());
}

void GameState::drawEntities()
{
	std::vector<flecs::entity> renderableEntities;

	const auto renderFilter = ECS::getWorld().filter<Render2D>();
	renderFilter.each([&](flecs::entity entity, Render2D renderer) {
		renderableEntities.push_back(entity);
		renderer.draw(&entity);
	});

	std::sort(renderableEntities.begin(), renderableEntities.end(),
		[](const flecs::entity& entity1, const flecs::entity& entity2) {
			const vf2d pos1 = entity1.get<RigidBody2D>()->RigidBody->GetPosition();
			const vf2d pos2 = entity2.get<RigidBody2D>()->RigidBody->GetPosition();
			return pos1.y < pos2.y;
		});

	for (auto& entity : renderableEntities) {
		entity.get_mut<Render2D>()->draw(&entity);
		
	}

	std::vector<flecs::entity> entitiesHealth;

	const auto healthFilter = ECS::getWorld().filter<Health>();
	healthFilter.each([&](flecs::entity entity, Health health) {

		health.draw(entity);

		if (health.currentHealth <= 0.0f) {
			entitiesHealth.push_back(entity);
			
		}
	});

	for (auto entity: entitiesHealth)
	{
		entity.set<DeleteMe>({});
	}

}

void GameState::update()
{

}

void GameState::handleInput()
{
	//only for global input


}

void GameState::drawUI()
{
	// UI
	if (Configuration::showGameStats) {
        UI::drawDebugInfo();
	}
    Log::resetLog();

	if (IsKeyPressed(KEY_N)) {




		resetGame();

	}





}
