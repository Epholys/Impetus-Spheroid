#include <iterator>

#include "framework/Assertion.hpp"
#include "engine/PhysicEngine.hpp"



namespace eg
{
//-----------------------------------------------------------------------------
// *** constructor ***
	PhysicEngine::PhysicEngine(ecs::EntityManager& ecs)
		: ecs_(ecs)
		, contacts_()
		, precision_(4)
	{
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

		for(unsigned int i=0; i<precision_; ++i)
		{
			handleAllCollisions(dt);
		}

		updateGravity(dt);

		updateMovement(dt);
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

			velComp->velocity_ += massComp->gravityVect_ * dt.asSeconds();
		}
	}


//-----------------------------------------------------------------------------
// *** Collisions functions: ***

	void PhysicEngine::handleAllCollisions(Time dt)
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
				auto firstSolidComp = dynCast<ecs::Solid>(sol1);
				auto secondSolidComp = dynCast<ecs::Solid>(sol2);
				assert(firstSolidComp);
				assert(secondSolidComp);

				auto firstVelComp = dynCast<ecs::Velocity>(vel1);
				auto secondVelComp = dynCast<ecs::Velocity>(vel2);

				/* If the Velocity Component is paused or doesn't exists, create
				 * a motionless and unmovable object (i.e. a object with null
				 * velocity and infinite mass */
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
				auto firstInvMass = firstSolidComp->invMass_;
				auto secondVel = secondVelComp->velocity_;
				auto secondInvMass = secondSolidComp->invMass_;

				auto impulse = computeImpulse(dt,
				                              contactNormal, contactDistance,
				                              firstVel, firstInvMass,
				                              secondVel, secondInvMass);

				if(impulse < 0)
				{
					saveTrackedCollision(entityPair);
				}

				firstVelComp->velocity_ += impulse * contactNormal * firstInvMass * firstSolidComp->restitution_;
				secondVelComp->velocity_ -= impulse * contactNormal * secondInvMass * secondSolidComp->restitution_;
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
				// Disgusting hack to jump over the case where firstIt == secondIt
				if (firstIt == secondIt)
				{
					++secondIt;
					if(secondIt==collidables.end())
						break;
				}

				auto firstPosComp = dynCast<ecs::Position>
					(firstIt->second.at(ecs::Component::Position));
				auto secondPosComp = dynCast<ecs::Position>
					(secondIt->second.at(ecs::Component::Position));
				assert(firstPosComp);
				assert(secondPosComp);


				auto firstCollSphere = dynCast<ecs::CollidableSphere>
					(firstIt->second.at(ecs::Component::Collidable));
				auto secondCollSphere = dynCast<ecs::CollidableSphere>
					(secondIt->second.at(ecs::Component::Collidable));
				auto firstCollRect = dynCast<ecs::CollidableRect>
					(firstIt->second.at(ecs::Component::Collidable));
				auto secondCollRect = dynCast<ecs::CollidableRect>
					(secondIt->second.at(ecs::Component::Collidable));

				Contact contact;
				bool contactGenerated = false;

				if(firstCollSphere && secondCollSphere)
				{
					contact =  generateSphereContact(firstPosComp->position_,
					                                 firstCollSphere->radius_,
					                                 secondPosComp->position_,
					                                 secondCollSphere->radius_);
					contactGenerated = true;
						                                       
				}
				else if((firstCollRect && secondCollSphere)
				        || (firstCollSphere && secondCollRect))
				{
					if(firstCollRect)
					{
						contact = generateMixedContact(secondPosComp->position_,
						                               secondCollSphere->radius_,
						                               firstPosComp->position_,
						                               firstCollRect->size_);
					}

					else
					{
						contact = generateMixedContact(firstPosComp->position_,
						                               firstCollSphere->radius_,
						                               secondPosComp->position_,
						                               secondCollRect->size_);
						contact.normal_ *= -1.f;
					}

					contactGenerated = true;
				}
				else
				{
					// Do nothing
				}

				/* If contact.normal_ is the null vector and can't be
				 * normalized, transform it into a arbitrary vector. 
				 * */
				if(!contact.normal_.normalize())
				{
					contact.normal_ = Vector2f(0.f, 1.f);
				}
					
				if(contactGenerated)
				{
					contacts_.emplace(std::make_pair(firstIt->first, secondIt->first),
					                  contact);
				}
			}
		}
	}


	Contact PhysicEngine::generateSphereContact(Vector2f firstPos, float firstRadius,
	                                            Vector2f secondPos, float secondRadius)
	{
		Contact contact;

		contact.normal_ = Vector2f(secondPos.x - firstPos.x,
		                           secondPos.y - firstPos.y);

		contact.distance_ = (std::sqrt(std::pow(contact.normal_.x, 2)
		                               + std::pow(contact.normal_.y, 2))
		                     - firstRadius
		                     - secondRadius);
		
		return contact;
	}

	Contact PhysicEngine::generateMixedContact(Vector2f spherePos, float sphereRadius,
	                                           Vector2f rectPos, Vector2f rectSize)
	{
		Contact contact;

		Vector2f gap (spherePos.x - rectPos.x, spherePos.y - rectPos.y);

		bool insideX = !(std::max(std::abs(gap.x) - rectSize.x / 2.f, 0.f));
		bool insideY = !(std::max(std::abs(gap.y) - rectSize.y / 2.f, 0.f));

		float xFactor = std::signbit(gap.x) ? -1 : 1;
		float yFactor = std::signbit(gap.y) ? -1 : 1;

		gap = Vector2f(gap.x - xFactor * rectSize.x / 2.f,
		               gap.y - yFactor * rectSize.y / 2.f);

		if(insideX)
			gap.x = 0.f;
		if(insideY)
			gap.y = 0.f;
		
		contact.normal_ = gap;

		contact.distance_ = (std::sqrt(std::pow(gap.x, 2) + std::pow(gap.y, 2))
		                     - sphereRadius);

		return contact;
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

				std::swap(pair.first, pair.second);
				trackedCollisions_.push_back(pair);
			}
			return;
		}
	}
	
} // namespace eg
