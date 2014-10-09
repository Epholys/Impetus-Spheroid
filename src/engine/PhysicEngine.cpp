#include <iostream>

#include "framework/Assertion.hpp"
#include "engine/PhysicEngine.hpp"



namespace eg
{
//-----------------------------------------------------------------------------
// *** constructor ***
	PhysicEngine::PhysicEngine(ecs::EntityManager& ecs)
		: ecs_(ecs)
		, contacts_()
		, gravityVect_(0.f, 1000.f)
	{
	}

//-----------------------------------------------------------------------------
// *** updates functions ***
	void PhysicEngine::update(Time dt)
	{
		updateGravity(dt);
		updateMovement(dt);

		handleCollisions(dt);
	}

	void PhysicEngine::updateGravity(Time dt)
	{
		auto massicObjects = ecs_.getObjectTable(ecs::Component::Massic);

		for(auto& massicPair : massicObjects)
		{
			auto velComp = dynCast<ecs::Velocity>
				(massicPair.second[ecs::Component::Velocity]);
			assert(velComp);

			auto massComp = dynCast<ecs::Mass>
				(massicPair.second[ecs::Component::Mass]);
			assert(massComp);

			/* I know this isn't scientifically correct, but for this game this
			is exactely what is needed
			*/
			velComp->velocity_ += massComp->mass_ * gravityVect_ * dt.asSeconds();
		}
	}


	void PhysicEngine::handleCollisions(Time dt)
	{
		auto objectTable = ecs_.getObjectTable(ecs::Component::Position | ecs::Component::Collidable);
		generateContacts(objectTable);		
	}

	void PhysicEngine::generateContacts
	(const ecs::EntityManager::objectTable& collidables)
	{
		contacts_.clear();
		for(auto firstIt = collidables.begin() ; firstIt!=collidables.end() ; ++firstIt)
		{
			for(auto secondIt = firstIt ; secondIt!=collidables.end() ; ++secondIt)
			{
				
				// Disgusting hack to jump over the cas when firstIt == secondIt
				if(firstIt!=secondIt)
				{
				
					auto firstPosComp = dynCast<ecs::Position>
						(firstIt->second.at(ecs::Component::Position));
					auto firstCollComp = dynCast<ecs::CollidableSphere>
						(firstIt->second.at(ecs::Component::Collidable));

					auto secondPosComp = dynCast<ecs::Position>
						(secondIt->second.at(ecs::Component::Position));
					auto secondCollComp = dynCast<ecs::CollidableSphere>
						(secondIt->second.at(ecs::Component::Collidable));

					assert(firstPosComp);
					assert(firstCollComp);
					assert(secondPosComp);
					assert(secondCollComp);


					Contact contact;

					contact.normal_ = Vector2f(secondPosComp->position_.x - firstPosComp->position_.x,
					                           secondPosComp->position_.y - firstPosComp->position_.y);
					contact.normal_.normalize();

					contact.distance_ =
						std::sqrt(std::pow(contact.normal_.x, 2) +
						          std::pow(contact.normal_.y, 2)) -
						firstCollComp->radius_ - 
						secondCollComp->radius_;


					contacts_.emplace(std::pair<ecs::Entity, ecs::Entity>(firstIt->first, secondIt->first),
					                  contact);
				}
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
			assert(posComp);

			auto velComp = dynCast<ecs::Velocity>
				(moveablePair.second[ecs::Component::Velocity]);
			assert(velComp);

			posComp->position_ += velComp->velocity_ * dt.asSeconds();
		}
	}

} // namespace eg
