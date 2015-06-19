#include "ecs/ComponentPhysic.hpp"


namespace ecs
{

	// *** Mass: ***

	Mass::Mass(float mass, Vector2f gravity)
		: mass_(mass)
		, gravityVect_(gravity)
	{
	}

	Component::Category Mass::getCategory() const
	{
		return Component::Mass;
	}

//-----------------------------------------------------------------------------
	// *** Solid: ***

	Solid::Solid(float invMass, float rest)
		: invMass_ (invMass), restitution_(0)
	{
		if (rest <= 0)
		{
			restitution_ = 0;
		}
		else
		{
			restitution_ = rest;
		}
	}

	Component::Category Solid::getCategory() const
	{
		return Component::Solid;
	}

//-----------------------------------------------------------------------------
	// *** Collidable: **

	Collidable::Collidable()
		: type(None)
	{
	}

	Collidable::Collidable(Type type)
		: type(type)
	{
	}
	
	Component::Category Collidable::getCategory() const
	{
		return Component::Collidable;
	}

} // namespace ecs
