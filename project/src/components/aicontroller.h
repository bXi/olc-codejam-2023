#pragma once

struct AIController
{
	flecs::entity target;
	flecs::entity self;

	float maxSpeed = 4.0f;

	flecs::entity selectRandomTarget() {

		std::vector<flecs::entity> entities;

		const auto playerFilter = ECS::getWorld().filter<PlayerIndex>();
		playerFilter.each([&](flecs::entity entity, PlayerIndex index) {
			entities.push_back(entity);
		});

		std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_int_distribution<int> distribution(0, static_cast<int>(entities.size()) - 1);
		return entities[distribution(generator)];
	}

	AIController()
	{
		target = selectRandomTarget();
	}

	void applyForce(const vf2d& desiredVelocity)
	{
		const b2Vec2 currentVelocity = self.get<RigidBody2D>()->RigidBody->GetLinearVelocity();
		const b2Vec2 steering = b2Vec2(desiredVelocity.x, desiredVelocity.y) - currentVelocity;
		const b2Vec2 force = b2Vec2(steering.x * maxSpeed, steering.y * maxSpeed);
		self.get<RigidBody2D>()->RigidBody->ApplyForceToCenter(force, true);
	}

	void seek()
	{
		const vf2d desired = vf2d(target.get<RigidBody2D>()->RigidBody->GetPosition() - self.get<RigidBody2D>()->RigidBody->GetPosition()).norm() * maxSpeed / Configuration::slowMotionFactor;
		applyForce(desired);
	}


};