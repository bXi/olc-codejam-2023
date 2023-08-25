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
    dungeonTileset = LoadTextureFromImage(LoadImage("assets/tilesets/dungeon.png"));

    Fonts::getFont("assets/fonts/APL386.ttf", 20);

    Audio::getMusic("assets/music/boss-1-loop.wav");
    Audio::getSound("assets/sfx/slowmo-enter.wav");
    Audio::getSound("assets/sfx/slowmo-exit.wav");


	screenWidth = static_cast<float>(GetScreenWidth());
	screenHeight = static_cast<float>(GetScreenHeight());
	screenRatio = screenWidth / screenHeight;

	World::generateNewMap();

	camera.offset = { static_cast<float>(GetScreenWidth()) / 2.0f, static_cast<float>(GetScreenHeight()) / 2.0f };
	camera.rotation = 0.0f;
	camera.zoom = 1.0f;

	resetGame();

	roomTitleLerp = Lerp::getLerp("roomTitleLerp", 0.0f, 10.0f, 4.0f);

	isLoaded = true;

}

void GameState::unload()
{}

void GameState::draw()
{
	if (!isLoaded) load();

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

		if (IsKeyPressed(KEY_F2)) input->weaponUpgrades[input->selectedWeapon - 1]++;
		if (IsKeyPressed(KEY_F3)) input->weaponUpgrades[input->selectedWeapon - 1] += 10;
		if (IsKeyPressed(KEY_F4)) input->weaponUpgrades[input->selectedWeapon - 1] += 100;


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


	if (World::getSlowMotionTimer() <= 0.0f) {
		Configuration::slowMotionFactor = 1.0f;
	}

	if (World::getSlowMotionTimer() <= 1.1f)
	{
		if (World::getPlaySlowMotionExit()) {
			Audio::playSound("assets/sfx/slowmo-exit.wav");
			World::setPlaySlowMotionExit(false);
		}
	}

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

	BeginMode2D(camera);

    widthMargin = 1.f + ((GetScreenWidth() / Configuration::tileWidth) / 2.f) / camera.zoom;
    heightMargin = 1.f + ((GetScreenHeight() / Configuration::tileHeight) / 2.f) / camera.zoom;

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
	if (IsKeyPressed(KEY_LEFT_BRACKET)) {
		camera.zoom = camera.zoom / 2.0f;
		if (camera.zoom < 0.5f) {
			camera.zoom = 0.5f;
		}
	}

	if (IsKeyPressed(KEY_RIGHT_BRACKET)) {
		camera.zoom = camera.zoom * 2.0f;
	}

	if (IsKeyPressed(KEY_M))
	{
		miniMapVisible = !miniMapVisible;
	}

	if (IsKeyPressed(KEY_F1)) {
		/*if (players.size() == 1) {
			players.emplace_back(new Wizard());

			players[1]->pos = players[0]->pos + vf2d({ 2.5f, 0.5f });
			players[1]->load();

			World::addObject(players[1]);

			setupControls();
		}*/
	}
}

void GameState::drawUI()
{
	// UI
	int playerId = 1;
	const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
	playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
        //UI::drawPlayerBar(entity, camera);
	});





	if (Configuration::showGameStats) {

        UI::drawDebugInfo();




	}
    Log::resetLog();

	if (IsKeyPressed(KEY_F10)) {
		


		World::clearObjects();
		World::generateNewMap();

		resetGame();

	}





}
