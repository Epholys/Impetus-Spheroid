#ifndef ECS_PHYSICENGINE_HPP
#define ECS_PHYSICENGINE_HPP


#include <memory>
#include <vector>
#include <map>

#include "utility/Time.hpp"
#include "ecs/Entity.hpp"
#include "ecs/ComponentBasic.hpp"
#include "ecs/ComponentPhysic.hpp"
#include "ecs/ComponentTracker.hpp"

namespace ecs
{
	
	/* This EntityManager is the link beetween the Entities, the Components, and
	 * the rest of the program. 
	 *
	 * TODO: Maybe try to optimise instead of copying everything.
	 *
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */

//-----------------------------------------------------------------------------


	class EntityManager
	{
	public:
		EntityManager();
		
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

	#include "ecs/EntityManager.ipp"
	
} // namespace ecs


#endif // ECS_PHYSICENGINE_HPP
