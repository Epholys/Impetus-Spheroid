#include <stdexcept>

#include "engine/PhysicEngine.hpp"


namespace eg
{
// *** Constructor: ***
	PhysicEngine::PhysicEngine()
		: entityCount_(0)
		, entityComponents_()
	{
	}

//-----------------------------------------------------------------------------
// *** ECS: ***
// ** Helper: **
	bool PhysicEngine::entityExists(Entity ent) const
	{
		return entityComponents_.count(ent) > 0;
	}

	bool PhysicEngine::componentExists(Entity ent, Component::Category cat) const
	{
		if(entityExists(ent))
		{
			return entityComponents_.at(ent).count(cat) > 0;
		}

		return false;
	}

// ** Entity: **
	Entity PhysicEngine::addEntity()
	{
		if(entityCount_ == maxNEntities-1)
		{
			throw std::runtime_error
				("PhysicEngine::addEntity(): max number of entities reached");
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
	void PhysicEngine::addComponent(Entity ent, ComponentBase::SPtr comp)
	{
		if(entityExists(ent))
		{
			entityComponents_[ent].emplace(comp->getCategory(), comp);
		}
	}

	void PhysicEngine::addComponents(Entity ent,
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


	void PhysicEngine::removeComponent(Entity ent, Component::Category cat)
	{
		if(componentExists(ent, cat))
		{
			entityComponents_[ent].erase(cat);
		} 
	}

	void PhysicEngine::removeAllComponents(Entity ent)
	{
		if(entityExists(ent))
		{
			entityComponents_[ent].clear();
		}
	}


	ComponentBase::SPtr PhysicEngine::getComponent(Entity ent, Component::Category cat)
	{
		if(componentExists(ent, cat))
		{
			return entityComponents_[ent][cat];
		}
		return ComponentBase::SPtr(nullptr);
	}

	const ComponentBase::SPtr PhysicEngine::getComponent(Entity ent,
	                                                     Component::Category cat) const
	{
		if(componentExists(ent, cat))
		{
			return entityComponents_.at(ent).at(cat);
		}
		return ComponentBase::SPtr(nullptr);
	}


	std::map<Component::Category, ComponentBase::SPtr>
	PhysicEngine::getAllComponents(Entity ent)
	{
		if(entityExists(ent))
		{
			return entityComponents_[ent];
		}
		return std::map<Component::Category, ComponentBase::SPtr>();
	}

	const std::map<Component::Category, ComponentBase::SPtr>
	PhysicEngine::getAllComponents(Entity ent) const
	{
		if(entityExists(ent))
		{
			return entityComponents_.at(ent);
		}
		return std::map<Component::Category, ComponentBase::SPtr>();
	}


	void PhysicEngine::pauseComponent(Entity ent, Component::Category cat, Time dt)
	{
		if(componentExists(ent, cat))
		{
			entityComponents_[ent][cat]->pause(dt);
		}
	}

	void PhysicEngine::pauseAllComponents(Entity ent, Time dt)
	{
		if(entityExists(ent))
		{
			for(const auto& pair : entityComponents_[ent])
			{
				pair.second->pause(dt);
			}
		}
	}

	void PhysicEngine::pauseAllComponents(Component::Category cat, Time dt)
	{
		for(const auto& entPair : entityComponents_)
		{
			if(componentExists(entPair.first, cat))
			{
				entPair.second.at(cat)->pause(dt);
			}
		}
	}

	void PhysicEngine::pauseAllComponents(Time dt)
	{
		for(const auto& entPair : entityComponents_)
		{
			for(const auto& compPair : entPair.second)
			{
				compPair.second->pause(dt);
			}
		}
	}


} // namespace eg
