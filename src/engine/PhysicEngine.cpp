#include "engine/PhysicEngine.hpp"


namespace eg
{
//-----------------------------------------------------------------------------
// *** constructor ***
	PhysicEngine::PhysicEngine(ecs::EntityManager& ecs)
		: ecs_(ecs)
		, gravityVect_(0.f, 9.8f)
	{
	}

//-----------------------------------------------------------------------------
// *** updates functions ***
	void PhysicEngine::update(Time dt)
	{
		updateGravity(dt);
		updateMovement(dt);
	}

	void PhysicEngine::updateGravity(Time)
	{
		auto massicObjects = ecs_.getObjectTable(ecs::Component::Massic);

		for(auto& massicPair : massicObjects)
		{
			auto velComp = dynCast<ecs::Velocity>
				(massicPair.second[ecs::Component::Velocity]);
				
			velComp->velocity_ += gravityVect_;
		}
	}

	void PhysicEngine::updateMovement(Time dt)
	{
		auto moveableObjects = ecs_.getObjectTable(ecs::Component::Moveable);

		for (auto& moveablePair : moveableObjects)
		{
			auto posComp = dynCast<ecs::Position>
				(moveablePair.second[ecs::Component::Position]);

			auto velComp = dynCast<ecs::Velocity>
				(moveablePair.second[ecs::Component::Velocity]);

			posComp->position_ += velComp->velocity_ * dt.asSeconds();
			velComp->velocity_ = Vector2f(0.f, 0.f);
		}
	}


} // namespace eg
