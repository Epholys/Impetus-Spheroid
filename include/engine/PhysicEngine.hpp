#ifndef EG_PHYSICENGINE_HPP
#define EG_PHYSICENGINE_HPP


#include <memory>
#include <vector>
#include <map>

#include "engine/Time.hpp"
#include "engine/Entity.hpp"
#include "engine/ComponentBasic.hpp"
#include "engine/ComponentPhysic.hpp"
#include "engine/ComponentTracker.hpp"

namespace eg
{
	
	/* This physics engine is a mix beetween an ECS and a traditionnal physic
	 * egine which uses speculative contacts for its semi-continuous collision
	 * engine.
	 *
	 * TODO: Maybe refactor everything to have several smalls classes instead of
	 * this behemoth.
	 * TODO: Maybe try to optimise instead of copying everything.
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

		/* Add an entity to entityComponents_
		 *
		 * Returns the Entity.
		 * */
		Entity addEntity();

		
		/* Constructs Comp with Args.
		 *
		 * If ent doesn't exist, does nothing.
		 * If ent already have a component of the same category, replace it.
		 * */
		template<typename Comp, typename... Arguments>
		void addComponent(Entity ent, Arguments... args);

 
		/* Add comp to ent.
		 *
		 * If ent doesn't exist, does nothing.
		 * If ent already have a component of the same category, replace it.
		 * */		 
		void addComponent(Entity ent, ComponentBase::SPtr comp);


		/* Add all comps' elements to ent.
		 *
		 * If ent doesn't exist, does nothing.
		 * If ent already have a component of the same category, replace it.
		 * */
		void addComponents(Entity ent,
		                   const std::vector<ComponentBase::SPtr>& comps);


		/* Remove the Component of category cat of ent.
		 *
		 * If ent doesn't exist, does nothing.
		 * If ent doesn't have a Component of category cat, does nothing.
		 * */
		void removeComponent(Entity ent, Component::Category cat);


		/* Remove all Components of ent
		 * 
		 * If ent doesn't exist, does nothing.
		 * */
		void removeAllComponents(Entity ent);


		/* (const) Getter for the component of category cat of ent.
		 *
		 * If ent doesn't exists or ent doesn't have a Component of category
		 * cat, returns a nullptr.
		 * */
		ComponentBase::SPtr getComponent(Entity ent, Component::Category cat);
		const ComponentBase::SPtr getComponent(Entity ent, Component::Category cat) const;


		/* (const) Getter for the std::map of Components of ent.
		 *
		 * If ent doesn't exists or doesn't have any Components, returns an
		 * empty std::map.
		 * */
		std::map<Component::Category, ComponentBase::SPtr>
		getAllComponents(Entity ent);

		const std::map<Component::Category, ComponentBase::SPtr>
		getAllComponents(Entity ent) const;


		/* Pauses ent's Component of category cat for dt.
		 *
		 * If ent doesn't exists of doesn't have any Component of category cat,
		 * does nothing. 
		 * */
		void pauseComponent(Entity ent, Component::Category cat, Time dt);

		/* Pauses all ent's Component for dt.
		 *
		 * If ent doesn't exists, does nothing.
		 * */
		void pauseAllComponents(Entity ent, Time dt);

		// Pauses all Components of category cat for dt.
		void pauseAllComponents(Component::Category cat, Time dt);

		// Pauses ALL Components for dt.
		void pauseAllComponents(Time dt);


	private:
		// Verify if ent exists in entityComponents_.
		bool entityExists(Entity ent) const;

		/* Verify if ent has a Component of category cat.
		 *
		 * If ent doesn't exist, return false.
		 * */
		bool componentExists(Entity ent, Component::Category cat) const;


	private:
		Entity entityCount_;

		std::map<Entity, std::map<Component::Category,
		                          ComponentBase::SPtr>>
			entityComponents_;

		
	};

	#include "engine/PhysicEngine.ipp"
	
} // namespace eg


#endif // EG_PHYSICENGINE_HPP
