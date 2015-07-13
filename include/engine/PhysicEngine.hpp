#ifndef EG_PHYSICENGINE_HPP
#define EG_PHYSICENGINE_HPP


#include <utility>
#include <map>
#include <unordered_map>
#include <functional>
#include <utility>

#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "utility/utility.hpp"
#include "ecs/EntityManager.hpp"
#include "engine/Contact.hpp"

namespace eg
{
	/* A simple continuous physic engine by using speculative contacts. Built
	 * on top of the ECS, for maximum flexibility.
	 *
	 * This engine is NOT intended to be a generalist physic engine, it will
	 * instead be specially build for this game.
	 *
	 * TODO: Add some cache system not to search at each frame all entities with
	 * X or Y Component.
	 * TODO: Optimize (if necessary)
	 * 
	 * More information physic engine and speculative contacts: 
	 * http://www.wildbunny.co.uk/blog/2011/03/25/speculative-contacts-an-continuous-collision-engine-approach-part-1/
	 * http://www.wildbunny.co.uk/blog/2011/04/06/physics-engines-for-dummies/
	 * */

//-----------------------------------------------------------------------------

	class PhysicEngine
	{
	public:
		typedef std::pair<ecs::Entity, ecs::Entity> entityPair;

		typedef
			std::map<entityPair, Contact> contactTable;

	public:
		PhysicEngine(ecs::EntityManager& ecs, int precision = 4);

		/* Main function called at each frame ; update everything. */
		void update(Time dt);

		/* Returns trackedCollisions_ */
		const std::vector<entityPair>& getTrackedCollisions() const;

		/* Set precision_ to precision.
		 * Asserts if precision > 0.
		 * */
		void setPrecision(int precision);

	private:
		/* Apply gravity */
		void updateGravity(Time dt);

		/* Apply movements */
		void updateMovement(Time dt);

		/* Resolves all collisions */
		void handleAllCollisions(Time dt);

		/* Generate all the Contacts of Collidable Entities of ecs_. */
		void generateAllContacts(const ecs::EntityManager::objectTable& collidables);

		/* Compute the impulse with the datas from handleAllCollisions.
		 * */
		float computeImpulse(Time dt,
		                     Vector2f contactNormal, float contactDistance,
		                     Vector2f firstVel, float firstInvMass,
		                     Vector2f secondVel, float secondInvMass);

		/* Save entityPair in trackedCollisions_ in the correct order */
		void saveTrackedCollision(entityPair pair);

	private:
		ecs::EntityManager& ecs_;

		contactTable contacts_;
		unsigned int precision_;

		/* The Projectile is the first in the entityPair, Target is the
		 * second. This attribute is cleared at each frame.*/
		std::vector<entityPair> trackedCollisions_;
	};

} // namespace eg

#endif //  EG_PHYSICENGINE_HPP
