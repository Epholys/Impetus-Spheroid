#include "engine/ComponentPhysic.hpp"


namespace eg
{

	// *** Mass: ***

	Mass::Mass(float mass)
		: mass_(mass)
	{
	}

	Component::Category Mass::getCategory() const
	{
		return Component::Mass;
	}

//-----------------------------------------------------------------------------
	// *** Solid: ***

	Solid::Solid(float rest)
		: restitution_(0)
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
	// *** Collidable and children: **

	CollidableSphere::CollidableSphere(float radius)
		: radius_(radius)
	{
	}

	Component::Category CollidableSphere::getCategory() const
	{
		return Component::Collidable;
	}

	
	CollidableRect::CollidableRect(Vector2f size)
		: size_(size)
	{
	}

	Component::Category CollidableRect::getCategory() const
	{
		return Component::Collidable;
	}


} // namespace eg
