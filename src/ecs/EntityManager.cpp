#include <stdexcept>

#include "ecs/EntityManager.hpp"


namespace ecs
{
// *** Constructor: ***
	EntityManager::EntityManager()
		: entityCount_(0)
		, objectTable_()
	{
	}

//-----------------------------------------------------------------------------
// *** Helpers: ***

	bool EntityManager::entityExists(Entity ent) const
	{
		return objectTable_.find(ent) != objectTable_.end();
	}

	bool EntityManager::componentExists(Entity ent, Component::Category cat) const
	{
		if(entityExists(ent))
		{
			return objectTable_.at(ent).find(cat) != objectTable_.at(ent).end();
		}

		return false;
	}

	bool EntityManager::componentIsActive(Entity ent, Component::Category cat) const
	{
		if(componentExists(ent, cat))
		{
			return !(objectTable_.at(ent).at(cat)->isPaused());
		}

		return false;
	}
	

//-----------------------------------------------------------------------------
// *** Entity: ***

	Entity EntityManager::addEntity()
	{
		if(entityCount_ == maxNEntities-1)
		{
			throw std::runtime_error
				("EntityManager::addEntity(): max number of entities reached");
		}
		else
		{
			++entityCount_;
			objectTable_.emplace(entityCount_,
			                          std::unordered_map<Component::Category, 
			                                             ComponentBase::SPtr,
			                                             std::hash<unsigned int>>());
			entityMasks_.emplace(entityCount_, 0);
			return entityCount_;
		}
	}

	void EntityManager::removeEntity(Entity ent)
	{
		if(entityExists(ent))
		{
			objectTable_.erase(ent);
			entityMasks_.erase(ent);
		}
	}

//-----------------------------------------------------------------------------
// *** Components: ***

	void EntityManager::addComponent(Entity ent, ComponentBase::SPtr comp)
	{
		if(entityExists(ent))
		{
			objectTable_[ent].emplace(comp->getCategory(), comp);
			entityMasks_[ent] = entityMasks_[ent] | comp->getCategory();
		}
	}

	void EntityManager::addComponents(Entity ent,
	                                 const std::vector<ComponentBase::SPtr>& comps)
	{
		if(entityExists(ent))
		{
			for(auto pComp : comps)
			{
				objectTable_[ent].emplace(pComp->getCategory(), pComp);
				entityMasks_[ent] = entityMasks_[ent] | pComp->getCategory();
			}
		}
	}


	void EntityManager::removeComponent(Entity ent, Component::Category cat)
	{
		if(componentExists(ent, cat))
		{
			objectTable_[ent].erase(cat);
			entityMasks_[ent] = entityMasks_[ent] - (entityMasks_[ent] & cat);
		} 
	}


	void EntityManager::removeAllComponents(Entity ent)
	{
		if(entityExists(ent))
		{
			objectTable_[ent].clear();
			entityMasks_[ent] = 0;
		}
	}


	ComponentBase::SPtr EntityManager::getComponent(Entity ent, Component::Category cat, bool force)
	{
		if(componentIsActive(ent, cat) ||
		   (force && componentExists(ent, cat)))
		{
			return objectTable_[ent][cat];
		}
		return ComponentBase::SPtr(nullptr);
	}

	const ComponentBase::SPtr EntityManager::getComponent(Entity ent,
	                                                      Component::Category cat,
	                                                      bool force) const
	{
		if(componentIsActive(ent, cat) ||
		   (force && componentExists(ent, cat)))
		{
			return objectTable_.at(ent).at(cat);
		}
		return ComponentBase::SPtr(nullptr);
	}


	EntityManager::componentTable
	EntityManager::getAllComponents(Entity ent, bool force)
	{
		componentTable table;
		if(entityExists(ent))
		{
			for(auto& componentPair : objectTable_[ent])
			{
				if(!componentPair.second->isPaused() || force)
				{
					table.insert(componentPair);
				}
			}
		}
		return table;
	}

	const EntityManager::componentTable
	EntityManager::getAllComponents(Entity ent, bool force) const
	{
		componentTable table;
		if(entityExists(ent))
		{
			for(const auto& componentPair : objectTable_.at(ent))
			{
				if(!componentPair.second->isPaused() || force)
				{
					table.insert(componentPair);
				}
			}
		}
		return table;
	}


	std::vector<ComponentBase::SPtr> 
	EntityManager::getAllComponents(Component::Category cat, bool force)
	{
		std::vector<ComponentBase::SPtr> table;
		for(auto& entityPair : objectTable_)
		{
			if(componentIsActive(entityPair.first, cat) ||
			   (force && componentExists(entityPair.first, cat)))
			{
				table.push_back(entityPair.second[cat]);
			}
		}
		return table;
	}

	const std::vector<ComponentBase::SPtr> 
	EntityManager::getAllComponents(Component::Category cat, bool force) const
	{
		std::vector<ComponentBase::SPtr> table;
		for(auto& entityPair : objectTable_)
		{
			if(componentIsActive(entityPair.first, cat) ||
			   (force && componentExists(entityPair.first, cat)))
			{
				table.push_back(entityPair.second.at(cat));
			}
		}
		return table;
	}

	void EntityManager::pauseComponent(Entity ent, Component::Category cat, Time dt)
	{
		if(componentExists(ent, cat))
		{
			objectTable_[ent][cat]->pause(dt);
		}
	}

	void EntityManager::pauseAllComponents(Entity ent, Time dt)
	{
		if(entityExists(ent))
		{
			for(const auto& pair : objectTable_[ent])
			{
				pair.second->pause(dt);
			}
		}
	}

	void EntityManager::pauseAllComponents(Component::Category cat, Time dt)
	{
		for(const auto& entPair : objectTable_)
		{
			if(componentExists(entPair.first, cat))
			{
				entPair.second.at(cat)->pause(dt);
			}
		}
	}

	void EntityManager::pauseAllComponents(Time dt)
	{
		for(const auto& entPair : objectTable_)
		{
			for(const auto& compPair : entPair.second)
			{
				compPair.second->pause(dt);
			}
		}
	}


//-----------------------------------------------------------------------------
// *** Objects: ***

	/* Yeah, I know, it's disgusting.
	 *
	 * The main difficulty here is all the checkings ; we must check:
	 * - if an Entity has the good Components
	 * - if one Component is paused (if so, objects MUST NOT contains this
	 * Entity and its Components)
	 * - if comps is empty (if so, an empty std::map MUST NOT be inserted into
	 * the objectTable. Else, the PhysicEngine will iterate over empty maps and
	 * create nullptr when trying to access a Component::SPtr that doesn't
	 * exist).
	 * 
	 * TODO: Refactoring
	 * */ 

	EntityManager::objectTable
	EntityManager::getObjectTable(Component::CategoryMask mask)
	{
		EntityManager::objectTable objects;

		// For each entity epair.first ...
		for(auto& epair : objectTable_)
		{

			// ... check if it has the good components ...
			if((entityMasks_[epair.first] & mask) == mask)
			{

				EntityManager::componentTable comps;

				// ... and get all the components desired.
				for(Component::CategoryMask cat = 1;
				    cat != Component::CategoryCount;
				    cat = cat << 1)
				{
					if(cat & mask)
					{
						Component::Category strictCat = Component::Category(cat);
						if(!(epair.second[strictCat]->isPaused()))
						{
							comps[strictCat]=epair.second[strictCat];
						}
						else
						{
							// If one Component is paused, aborts.
							comps.clear();
							break;
						}
					}
				}
				
				if(!comps.empty())
				{
					objects[epair.first] = comps;
				}
			}
		}

		return objects;
	}

//-----------------------------------------------------------------------------
// *** Pause Mechanics: ***
	void EntityManager::update(Time dt)
	{
		for(auto& entityPair : objectTable_)
		{
			for(auto& componentPair : entityPair.second)
			{
				componentPair.second->update(dt);
			}
		}
	}


} // namespace ecs
