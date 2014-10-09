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
		, precision_(1)
		, gravityVect_(0.f, 1000.f)
	{
	}

//-----------------------------------------------------------------------------
// *** updates functions ***
	void PhysicEngine::update(Time dt)
	{
		generateContacts(ecs_.getObjectTable(ecs::Component::Position | ecs::Component::Collidable));
		for(unsigned int i=0; i<precision_; ++i)
		{
			handleCollisions(dt);
		}

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
		for(auto& contactPair : contacts_)
		{
			auto entityPair = contactPair.first;

			auto vel1 = ecs_.getComponent(entityPair.first, ecs::Component::Velocity);
			auto vel2 = ecs_.getComponent(entityPair.second, ecs::Component::Velocity);
			auto sol1 = ecs_.getComponent(entityPair.first, ecs::Component::Solid);
			auto sol2 = ecs_.getComponent(entityPair.second, ecs::Component::Solid);

			if(sol1 && sol2)
			{
				auto contactNormal = contactPair.second.normal_;
				auto contactDistance = contactPair.second.distance_;
				auto contactImpulse = contactPair.second.impulse_;

				auto firstSolidComp = dynCast<ecs::Solid>(sol1);
				auto secondSolidComp = dynCast<ecs::Solid>(sol2);
				assert(firstSolidComp);
				assert(secondSolidComp);

				auto firstVelComp = dynCast<ecs::Velocity>(vel1);
				auto secondVelComp = dynCast<ecs::Velocity>(vel2);
				/* If the Velocity Component is paused, create a still and
				 * unmovable object (i.e. a object with null velocity and
				 * infinite mass */
				if(!firstVelComp)
				{
					firstVelComp = std::make_shared<ecs::Velocity>(Vector2f(0.f,0.f));
					firstSolidComp = std::make_shared<ecs::Solid>(0.f, 1.f);
				}
				if(!secondVelComp)
				{
					secondVelComp = std::make_shared<ecs::Velocity>(Vector2f(0.f,0.f));
					secondSolidComp = std::make_shared<ecs::Solid>(0.f, 1.f);
				}

				Vector2f relativeVelocity = secondVelComp->velocity_ - firstVelComp->velocity_;
				float relativeNormalVelocity = relativeVelocity.dotProduct(contactNormal);

				float remove = relativeNormalVelocity + contactDistance / dt.asSeconds();

				float impulse = remove / (firstSolidComp->invMass_ + secondSolidComp->invMass_);

				float newImpulse = std::min(impulse + contactImpulse, 0.f);

				float change = newImpulse - contactImpulse;

				contactPair.second.impulse_ = newImpulse;

				firstVelComp->velocity_ += change * contactNormal * firstSolidComp->invMass_ * firstSolidComp->restitution_;
				secondVelComp->velocity_ -= change * contactNormal * secondSolidComp->invMass_ * secondSolidComp->restitution_;
			}
		}
	}

	void PhysicEngine::generateContacts
	(const ecs::EntityManager::objectTable& collidables)
	{
		contacts_.clear();
		for(auto firstIt = collidables.begin() ; firstIt!=collidables.end() ; ++firstIt)
		{
			for(auto secondIt = firstIt ; secondIt!=collidables.end() ; ++secondIt)
			{
				
				// Disgusting hack to jump over the case where firstIt == secondIt
				if(firstIt != secondIt)
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

					contact.distance_ =
						std::sqrt(std::pow(contact.normal_.x, 2) +
						          std::pow(contact.normal_.y, 2)) -
						firstCollComp->radius_ - 
						secondCollComp->radius_;

					contact.normal_.normalize();

					contact.impulse_ = 0.f;


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
