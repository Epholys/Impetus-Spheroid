#include "ecs/ComponentTracker.hpp"


namespace ecs
{

	// *** Target: ***

	Target::Target()
	{
	}

	Component::Category Target::getCategory() const
	{
		return Component::Target;
	}

//-----------------------------------------------------------------------------
	// *** Projectile: ***

	Projectile::Projectile()
		: hasTouchedTarget_(false)
	{
	}

	void Projectile::touchTarget()
	{
		hasTouchedTarget_ = true;
	}

	bool Projectile::hasTouchedTarget() const
	{
		return hasTouchedTarget_;
	}

	Component::Category Projectile::getCategory() const
	{
		return Component::Projectile;
	}


} // namespace ecs
