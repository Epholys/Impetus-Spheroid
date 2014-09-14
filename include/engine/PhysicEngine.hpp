#ifndef EG_PHYSICENGINE_HPP
#define EG_PHYSICENGINE_HPP


#include <memory>
#include <vector>
#include <map>

#include "Entity.hpp"
#include "ComponentBasic.hpp"
#include "ComponentPhysic.hpp"
#include "ComponentTracker.hpp"

namespace eg
{
	
	/* This physics engine is a mix beetween an ECS and a traditionnal physic
	 * egine which uses speculative contacts for its semi-continuous collision
	 * engine.
	 *
	 * TODO: Maybe refactor everything to have several smalls classes instead of
	 * this behemoth.
	 *
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 *
	 * For more informations about this engine, see:
	 * http://www.wildbunny.co.uk/blog/2011/04/06/physics-engines-for-dummies/
	 * http://www.wildbunny.co.uk/blog/2011/03/25/speculative-contacts-an-continuous-collision-engine-approach-part-1/
	 * */

//-----------------------------------------------------------------------------

	class PhysicEngine
	{
	public:
		PhysicEngine();
		
//-----------------------------------------------------------------------------
// *** ECS: ***

		// Add an entity to entityComponents_ and returns it.
		Entity addEntity();
		
		// Constructs Comp with Args and add it to ent.
		template<typename Comp, typename... Arguments>
		void addComponent(Entity ent, Arguments... args);

		// Add comp to ent.
		void addComponent(Entity ent, ComponentBase::SPtr comp);

		// Add all comps' elements to ent.
		void addComponents(Entity ent,
		                   const std::vector<ComponentBase::SPtr>& comps);

	private:
		// Verify if an ent exists in entityComponents_.
		bool entityExists(Entity ent);


	private:
		Entity entityCount_;
		std::map<Entity, std::vector<ComponentBase::SPtr>> entityComponents_;
	};

	#include "engine/PhysicEngine.ipp"
	
} // namespace eg


#endif // EG_PHYSICENGINE_HPP
