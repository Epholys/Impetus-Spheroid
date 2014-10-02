#include <iostream>

#include "engine/PhysicEngine.hpp"



namespace eg
{
//-----------------------------------------------------------------------------
// *** constructor ***
	PhysicEngine::PhysicEngine(ecs::EntityManager& ecs)
		: ecs_(ecs)
		, gravityVect_(0.f, 1000.f)
	{
	}

//-----------------------------------------------------------------------------
// *** updates functions ***
	void PhysicEngine::update(Time dt)
	{
		updateGravity(dt);
		updateMovement(dt);
	}

	void PhysicEngine::updateGravity(Time dt)
	{
		auto massicObjects = ecs_.getObjectTable(ecs::Component::Massic);

		for(auto& massicPair : massicObjects)
		{
			auto velComp = dynCast<ecs::Velocity>
				(massicPair.second[ecs::Component::Velocity]);
			
			if(velComp)
			{
				velComp->velocity_ += gravityVect_ * dt.asSeconds();
			}
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

			if (posComp && velComp)
			{
				posComp->position_ += velComp->velocity_ * dt.asSeconds();
			}
		}
	}


} // namespace eg
