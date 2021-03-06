#ifndef ECS_ENTITYMANAGER_HPP
#define ECS_ENTITYMANAGER_HPP

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>

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
	 * NOTE : An 'object' is an Entity with all its Components.
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
		// If Component::Category or Entity became something other than aliases
		// for existing type, uncomment this.

		// typedef std::map<Component::Category,
		//                   ComponentBase::SPtr> componentTable;

		// typedef std::map<Entity, componentTable> objectTable;

		typedef std::unordered_map<Component::Category,
		                           ComponentBase::SPtr,
		                           std::hash<unsigned int>> componentTable;

		typedef std::unordered_map<Entity,
		                           componentTable,
		                           std::hash<unsigned int>> objectTable;


	public:
		EntityManager();
		
//-----------------------------------------------------------------------------
// *** Entity: ***

		/* Add an entity to entityComponents_
		 *
		 * Returns the Entity.
		 * */
		Entity addEntity();


		/* Remove ent and all its Components from entityComponents_
		 *
		 * If ent doesn't exist, does nothing.
		 * */
		void removeEntity(Entity ent);

//-----------------------------------------------------------------------------
// *** Components: ***

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


		/* (const) Getter for the Component of category cat of ent.
		 *
		 * Returns the Component if it is paused only if force == true.
		 *
		 * If ent doesn't exists or ent doesn't have a Component of category
		 * cat, returns a nullptr.
		 * */
		ComponentBase::SPtr getComponent(Entity ent, Component::Category cat, bool force=false);
		const ComponentBase::SPtr getComponent(Entity ent, Component::Category cat, bool force=false) const;


		/* (const) Getter for the std::map of Components of ent.
		 *
		 * Returns a Component if it is paused only if force == true
		 *
		 * If ent doesn't exists or doesn't have any Components, returns an
		 * empty std::map.
		 * */
		componentTable getAllComponents(Entity ent, bool force=false);
		const componentTable getAllComponents(Entity ent, bool force=false) const;


		/* (const) Getter for all Components of category cat.
		 * 
		 * Returns a Component if it is paused only if force == true.
		 *
		 * If there isn't any Components of this category, returns an empty
		 * std::map.
		 */
		std::vector<ComponentBase::SPtr> getAllComponents(Component::Category cat, bool force=false);
		const std::vector<ComponentBase::SPtr> getAllComponents(Component::Category cat, bool force=false) const;



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

//-----------------------------------------------------------------------------
// *** Objects: ***
		
		/* Gets a table of all Entities and Component with all the Component
		 * choosen by mask.
		 * For example, get the table of all entities, with their associated
		 * Components, which have a Position and a Velocity. 
		 *
		 * Only the desired Components are in the table.
		 *
		 * If there isn't any entities with all Components of mask, return a
		 * empty objectTable.
		 * */
		objectTable getObjectTable(Component::CategoryMask mask);


		/* Gets a table of all Entities and Component with all the Component
		 * choosen by mask, EVEN IS THIS ENTITY DOESN'T HAVE THIS COMPONENT.
		 *
		 * Only the desired Components are in the table.
		 * If a Component asked is paused or doesn't exists, the Entity
		 * associated is given anyway
		 *
		 * If there isn't any entities with all Components of mask, return a
		 * empty objectTable.
		 * */

		objectTable getLooseObjectTable(Component::CategoryMask mask);

		
//-----------------------------------------------------------------------------
// *** Pause Mechanics: ***
		void update(Time dt);
		

	private:
		// Verify if ent exists in entityComponents_.
		bool entityExists(Entity ent) const;

		/* Verify if ent has a Component of category cat.
		 *
		 * If ent doesn't exist, return false.
		 * */
		bool componentExists(Entity ent, Component::Category cat) const;

		/* Verify if ent has a Component of category cat and if this Component
		 * isn't paused.
		 *
		 * If ent or the Component doesn't exist, return false.
		 * */
		bool componentIsActive(Entity ent, Component::Category cat) const;


	private:
		Entity entityCount_;

		objectTable objectTable_;

		// std::map<Entity, Component::CategoryMask> entityMasks_;
		std::unordered_map<Entity,
		                   Component::CategoryMask,
		                   std::hash<unsigned int>> entityMasks_;

		
	};

	#include "ecs/EntityManager.ipp"
	
} // namespace ecs


#endif // ECS_ENTITYMANAGER_HPP
