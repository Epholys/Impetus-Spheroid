#include "engine/ComponentTracker.hpp"


namespace eg
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


} // namespace eg
