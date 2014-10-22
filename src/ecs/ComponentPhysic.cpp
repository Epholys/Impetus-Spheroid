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


} // namespace ecs
