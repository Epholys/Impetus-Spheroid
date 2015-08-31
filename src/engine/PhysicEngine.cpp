#include <iterator>

#include "framework/Assertion.hpp"
#include "engine/PhysicEngine.hpp"



namespace eg
{
//-----------------------------------------------------------------------------
// *** default construction value: ***
	namespace
	{
		const int DEFAULT_PRECISION = 1;
	}

//-----------------------------------------------------------------------------
// *** constructor ***
	PhysicEngine::PhysicEngine(ecs::EntityManager& ecs, int precision)
		: ecs_(ecs)
		, contacts_()
		, precision_(DEFAULT_PRECISION)
	{
		assert(precision > 0);
		precision_ = precision;
	}

//-----------------------------------------------------------------------------
// *** public function: ***

	const std::vector<PhysicEngine::entityPair>&
	PhysicEngine::getTrackedCollisions() const
	{
		return trackedCollisions_;
	}

	void PhysicEngine::setPrecision(int precision)
	{
		assert(precision > 0);
		precision_ = precision;
	}

//-----------------------------------------------------------------------------
// *** updates functions ***
	void PhysicEngine::update(Time dt)
	{
		trackedCollisions_.clear();

		generateAllContacts(ecs_.getObjectTable(ecs::Component::Position | ecs::Component::Collidable));

		auto bounceable = ecs_.getLooseObjectTable(ecs::Component::Velocity | ecs::Component::Solid | ecs::Component::TimeArrow);
		for(unsigned int i=0; i<precision_; ++i)
		{
			handleAllCollisions(bounceable, dt);
		}

		updateGravity(dt);

		updateMovement(dt);
	}

	void PhysicEngine::updateMovement(Time dt)
	{
		auto moveableObjects = ecs_.getObjectTable(ecs::Component::Moveable);

		for (auto& moveablePair : moveableObjects)
		{
			Time deltaTime = dt;
			
			auto timeComp = dynCast<ecs::TimeArrow>
				(ecs_.getComponent(moveablePair.first, ecs::Component::TimeArrow));
			if(timeComp)
			{
				deltaTime *= timeComp->timeCoefficient_;
			}
			
			auto posComp = dynCast<ecs::Position>
				(moveablePair.second[ecs::Component::Position]);

			auto velComp = dynCast<ecs::Velocity>
				(moveablePair.second[ecs::Component::Velocity]);

			if(posComp && velComp)
				posComp->position_ += velComp->velocity_ * deltaTime.asSeconds();
		}
	}

	void PhysicEngine::updateGravity(Time dt)
	{
		auto massicObjects = ecs_.getObjectTable(ecs::Component::Massic);

		for(auto& massicPair : massicObjects)
		{
			Time deltaTime = dt;
			
			auto timeComp = dynCast<ecs::TimeArrow>
				(ecs_.getComponent(massicPair.first, ecs::Component::TimeArrow));
			if(timeComp)
			{
				deltaTime *= timeComp->timeCoefficient_;
			}

			auto velComp = dynCast<ecs::Velocity>
				(massicPair.second[ecs::Component::Velocity]);
			assert(velComp);

			auto massComp = dynCast<ecs::Mass>
				(massicPair.second[ecs::Component::Mass]);
			assert(massComp);

			velComp->velocity_ += massComp->gravityVect_ * deltaTime.asSeconds();
		}
	}


//-----------------------------------------------------------------------------
// *** Collisions functions: ***

	void PhysicEngine::handleAllCollisions(const ecs::EntityManager::objectTable& bounceable, Time dt)
	{
		for(auto& contactPair : contacts_)
		{
			auto entityPair = contactPair.first;

			ecs::ComponentBase::SPtr vel1, vel2, sol1, sol2;

			vel1 = bounceable.at(entityPair.first).at(ecs::Component::Velocity);
			vel2 = bounceable.at(entityPair.second).at(ecs::Component::Velocity);
			sol1 = bounceable.at(entityPair.first).at(ecs::Component::Solid);
			sol2 = bounceable.at(entityPair.second).at(ecs::Component::Solid);
			
			auto firstSolidComp = dynCast<ecs::Solid>(sol1);
			auto secondSolidComp = dynCast<ecs::Solid>(sol2);

			/* If the Solid Component is paused or doesn't exists, create a fake
			 * one to allow calculation of impulse. To see if the objects
			 * intersects with eachother without bouncing. */
			if(!firstSolidComp)
			{
				firstSolidComp = std::make_shared<ecs::Solid>(1.f, 1.f);
			}
			if(!secondSolidComp)
			{
				secondSolidComp = std::make_shared<ecs::Solid>(1.f, 1.f);
			}


			auto firstVelComp = dynCast<ecs::Velocity>(vel1);
			auto secondVelComp = dynCast<ecs::Velocity>(vel2);

			/* If the Velocity Component is paused or doesn't exists, create a
			 * motionless and unmovable object (i.e. a object with null velocity
			 * and infinite mass) */
			if(!firstVelComp)
			{
				firstVelComp = std::make_shared<ecs::Velocity>(Vector2f(0.f,0.f));
				firstSolidComp = std::make_shared<ecs::Solid>(0.f, 0.f);
			}
			if(!secondVelComp)
			{
				secondVelComp = std::make_shared<ecs::Velocity>(Vector2f(0.f,0.f));
				secondSolidComp = std::make_shared<ecs::Solid>(0.f, 0.f);
			}

			auto contactNormal = contactPair.second.normal_;
			auto contactDistance = contactPair.second.distance_;
				
			auto firstVel = firstVelComp->velocity_;
			auto firstTimeComp = dynCast<ecs::TimeArrow>
				(bounceable.at(entityPair.first).at(ecs::Component::TimeArrow));
			if(firstTimeComp)
			{
				firstVel *= firstTimeComp->timeCoefficient_;
			}
			auto firstInvMass = firstSolidComp->invMass_;

			auto secondVel = secondVelComp->velocity_;
			auto secondTimeComp = dynCast<ecs::TimeArrow>
				(bounceable.at(entityPair.second).at(ecs::Component::TimeArrow));
			if(secondTimeComp)
			{
				secondVel *= secondTimeComp->timeCoefficient_;
			}
			auto secondInvMass = secondSolidComp->invMass_;

			auto impulse = computeImpulse(dt,
			                              contactNormal, contactDistance,
			                              firstVel, firstInvMass,
			                              secondVel, secondInvMass);

			if(impulse < 0)
			{
				saveTrackedCollision(entityPair);
			}

			if(sol1 && sol2)
			{
				auto firstVelAddition = impulse * contactNormal * firstInvMass * firstSolidComp->restitution_;
				if(firstTimeComp)
				{
					firstVelAddition *= firstTimeComp->timeCoefficient_;
				}
				firstVelComp->velocity_ += firstVelAddition;

				auto secondVelAddition = impulse * contactNormal * secondInvMass * secondSolidComp->restitution_;
				if(secondTimeComp)
				{
					secondVelAddition *= secondTimeComp->timeCoefficient_;
				}
				secondVelComp->velocity_ -= secondVelAddition;
			}
		}
	}

	void PhysicEngine::generateAllContacts
	(const ecs::EntityManager::objectTable& collidables)
	{
		contacts_.clear();

		for(auto firstIt = collidables.begin() ; firstIt!=collidables.end() ; ++firstIt)
		{
			for(auto secondIt = firstIt ; secondIt!=collidables.end() ; ++secondIt)
			{	
				// Little hack to jump over the case where firstIt == secondIt,
				// as I can't do iterator arithmetic with bidirectional_iterator
				if (firstIt == secondIt)
				{
					++secondIt;
					if(secondIt==collidables.end())
						break;
				}

				auto firstPos = dynCast<ecs::Position>
					(firstIt->second.at(ecs::Component::Position));
				auto secondPos = dynCast<ecs::Position>
					(secondIt->second.at(ecs::Component::Position));
				auto firstColl = dynCast<ecs::Collidable>
					(firstIt->second.at(ecs::Component::Collidable));
				auto secondColl = dynCast<ecs::Collidable>
					(secondIt->second.at(ecs::Component::Collidable));
				assert(firstPos);
				assert(secondPos);
				assert(firstColl);
				assert(secondColl);

				
				Contact contact (firstPos->position_,
				                 secondPos->position_,
				                 *firstColl,
				                 *secondColl);

				/* If contact.normal_ is the null vector and can't be
				 * normalized, transform it into a arbitrary vector. 
				 * */
				if(!contact.normal_.normalize())
				{
					contact.normal_ = Vector2f(0.f, 0.f);
				}
					
				if(contact.defined_)
				{
					contacts_.emplace(std::make_pair(firstIt->first, secondIt->first),
					                  contact);
				}
			}
		}
	}

	float PhysicEngine::computeImpulse(Time dt,
	                                   Vector2f contactNormal, float contactDistance,
	                                   Vector2f firstVel, float firstInvMass,
	                                   Vector2f secondVel, float secondInvMass)
	{
		// Two solid with infinite mass acts like two solids of mass 1
		if(!firstInvMass && !secondInvMass)
		{
			firstInvMass = 1.f;
			secondInvMass = 1.f;
		}

		Vector2f relativeVelocity = secondVel - firstVel;
		float relativeNormalVelocity = relativeVelocity.dotProduct(contactNormal);

		float remove = relativeNormalVelocity + contactDistance / dt.asSeconds();

		float impulse = remove / (firstInvMass + secondInvMass);

		return std::min(impulse, 0.f);
	}
		                     

	void PhysicEngine::saveTrackedCollision(entityPair pair)
	{
		auto proj1 = ecs_.getComponent(pair.first, ecs::Component::Projectile);
		auto targ2 = ecs_.getComponent(pair.second, ecs::Component::Target);

		auto firstProjComp = dynCast<ecs::Projectile>(proj1);
		auto secondTargetComp = dynCast<ecs::Target>(targ2);
		
		if(firstProjComp && secondTargetComp)
		{
			if(!firstProjComp->hasTouchedTarget())
			{
				firstProjComp->touchTarget();
				secondTargetComp->touch();

				trackedCollisions_.push_back(pair);
			}
			return;
		}


		auto proj2 = ecs_.getComponent(pair.second, ecs::Component::Projectile);
		auto targ1 = ecs_.getComponent(pair.first, ecs::Component::Target);

		auto secondProjComp = dynCast<ecs::Projectile>(proj2);
		auto firstTargetComp = dynCast<ecs::Target>(targ1);

		if(secondProjComp && firstTargetComp)
		{
			if(!secondProjComp->hasTouchedTarget())
			{
				secondProjComp->touchTarget();
				firstTargetComp->touch();

				std::swap(pair.first, pair.second);
				trackedCollisions_.push_back(pair);
			}
			return;
		}
	}
	
} // namespace eg
