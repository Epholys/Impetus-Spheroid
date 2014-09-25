#ifndef EG_PHYSICENGINE_HPP
#define EG_PHYSICENGINE_HPP


#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "ecs/EntityManager.hpp"


namespace eg
{
	/* A simple continuous physic engine by using speculative contacts. Built
	 * on top of the ECS, for maximum flexibility.
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
		PhysicEngine(ecs::EntityManager& ecs);

		/* Main functtion called at each frame ; update everything. */
		void update(Time dt);

	private:
		/* Just to have a shorter name that
		 * std::dynamic_pointer_cast<...>()
		 * */
		template<typename Child>
		std::shared_ptr<Child> dynCast(const ecs::ComponentBase::SPtr& pParent);
			

		/* Apply gravity */
		void updateGravity(Time dt);

		/* Apply movements */
		void updateMovement(Time dt);

	private:
		ecs::EntityManager& ecs_;

		Vector2f gravityVect_;
	};

#include "engine/PhysicEngine.ipp"

} // namespace eg

#endif //  EG_PHYSICENGINE_HPP
