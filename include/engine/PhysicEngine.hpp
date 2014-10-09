#ifndef EG_PHYSICENGINE_HPP
#define EG_PHYSICENGINE_HPP


#include <utility>
#include <map>

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
	 * 
	 * More information physic engine and speculative contacts: 
	 * http://www.wildbunny.co.uk/blog/2011/03/25/speculative-contacts-an-continuous-collision-engine-approach-part-1/
	 * http://www.wildbunny.co.uk/blog/2011/04/06/physics-engines-for-dummies/
	 * */


	class PhysicEngine
	{
	public:
		typedef
			std::map<std::pair<ecs::Entity, ecs::Entity>, Contact>
			contactTable;

	public:
		PhysicEngine(ecs::EntityManager& ecs);

		/* Main functtion called at each frame ; update everything. */
		void update(Time dt);

	private:
		/* Apply gravity */
		void updateGravity(Time dt);

		/* Generate all the Contacts of Collidable Entities of ecs_ */
		void handleCollisions(Time dt);
		void generateContacts(const ecs::EntityManager::objectTable& collidables);

		/* Apply movements */
		void updateMovement(Time dt);


	private:
		ecs::EntityManager& ecs_;

		contactTable contacts_;

		Vector2f gravityVect_;
	};

} // namespace eg

#endif //  EG_PHYSICENGINE_HPP
