#include <stdexcept>

#include "ecs/EntityManager.hpp"


namespace ecs
{
// *** Constructor: ***
	EntityManager::EntityManager()
		: entityCount_(0)
		, entityComponents_()
	{
	}

//-----------------------------------------------------------------------------
// *** ECS: ***
// ** Helper: **
	bool EntityManager::entityExists(Entity ent) const
	{
		return entityComponents_.count(ent) > 0;
	}

	bool EntityManager::componentExists(Entity ent, Component::Category cat) const
	{
		if(entityExists(ent))
		{
			return entityComponents_.at(ent).count(cat) > 0;
		}

		return false;
	}

// ** Entity: **
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
			entityComponents_.emplace(entityCount_,
			                          std::map<Component::Category, 
			                                   ComponentBase::SPtr>());
			return entityCount_;
		}
	}

// ** Components: **
	void EntityManager::addComponent(Entity ent, ComponentBase::SPtr comp)
	{
		if(entityExists(ent))
		{
			entityComponents_[ent].emplace(comp->getCategory(), comp);
		}
	}

	void EntityManager::addComponents(Entity ent,
	                                 const std::vector<ComponentBase::SPtr>& comps)
	{
		if(entityExists(ent))
		{
			for(auto pComp : comps)
			{
				entityComponents_[ent].emplace(pComp->getCategory(), pComp);
			}
		}
	}


	void EntityManager::removeComponent(Entity ent, Component::Category cat)
	{
		if(componentExists(ent, cat))
		{
			entityComponents_[ent].erase(cat);
		} 
	}

	void EntityManager::removeAllComponents(Entity ent)
	{
		if(entityExists(ent))
		{
			entityComponents_[ent].clear();
		}
	}


	ComponentBase::SPtr EntityManager::getComponent(Entity ent, Component::Category cat)
	{
		if(componentExists(ent, cat))
		{
			return entityComponents_[ent][cat];
		}
		return ComponentBase::SPtr(nullptr);
	}

	const ComponentBase::SPtr EntityManager::getComponent(Entity ent,
	                                                     Component::Category cat) const
	{
		if(componentExists(ent, cat))
		{
			return entityComponents_.at(ent).at(cat);
		}
		return ComponentBase::SPtr(nullptr);
	}


	std::map<Component::Category, ComponentBase::SPtr>
	EntityManager::getAllComponents(Entity ent)
	{
		if(entityExists(ent))
		{
			return entityComponents_[ent];
		}
		return std::map<Component::Category, ComponentBase::SPtr>();
	}

	const std::map<Component::Category, ComponentBase::SPtr>
	EntityManager::getAllComponents(Entity ent) const
	{
		if(entityExists(ent))
		{
			return entityComponents_.at(ent);
		}
		return std::map<Component::Category, ComponentBase::SPtr>();
	}


	void EntityManager::pauseComponent(Entity ent, Component::Category cat, Time dt)
	{
		if(componentExists(ent, cat))
		{
			entityComponents_[ent][cat]->pause(dt);
		}
	}

	void EntityManager::pauseAllComponents(Entity ent, Time dt)
	{
		if(entityExists(ent))
		{
			for(const auto& pair : entityComponents_[ent])
			{
				pair.second->pause(dt);
			}
		}
	}

	void EntityManager::pauseAllComponents(Component::Category cat, Time dt)
	{
		for(const auto& entPair : entityComponents_)
		{
			if(componentExists(entPair.first, cat))
			{
				entPair.second.at(cat)->pause(dt);
			}
		}
	}

	void EntityManager::pauseAllComponents(Time dt)
	{
		for(const auto& entPair : entityComponents_)
		{
			for(const auto& compPair : entPair.second)
			{
				compPair.second->pause(dt);
			}
		}
	}


} // namespace ecs
