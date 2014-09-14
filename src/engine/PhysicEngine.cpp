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
	bool entityExists(Entity ent)
	{
		return entityComponents_.count(ent) > 0;
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
			                          std::vector<ComponentBase::SPtr>());
			return entityCount_;
		}
	}

// ** Components: **
	void PhysicEngine::addComponent(Entity ent, ComponentBase::SPtr comp)
	{
		if(entityExists(ent))
		{
			entityComponents_[ent].push_back(comp);
		}
	}

	void PhysicEngine::addComponents(Entity ent,
	                                 const std::vector<ComponentBase::SPtr>& comps);
	{
		if(entityExists(ent))
		{
			for(auto pComp : comps)
			{
				entityComponents_[ent].push_back(pComp);
			}
		}
	}

} // namespace eg
