#include "ecs/ComponentTracker.hpp"


namespace ecs
{

	// *** Target: ***

	Target::Target()
		: hasBeenTouched_(false)
	{
	}

	Component::Category Target::getCategory() const
	{
		return Component::Target;
	}

	void Target::touch()
	{
		hasBeenTouched_ = true;
	}
	
	bool Target::reset()
	{
		bool returnVal = hasBeenTouched_;
		hasBeenTouched_ = false;
		return returnVal;
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
