#include "ecs/ComponentTracker.hpp"


namespace ecs
{

	// *** Target: ***

	Target::Target(float pointMultiplier)
		: hasBeenTouched_(false)
		, pointMultiplier_(pointMultiplier)
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

	float Target::getPointMultiplier() const
	{
		return pointMultiplier_;
	}

//-----------------------------------------------------------------------------
	// *** Projectile: ***

	Projectile::Projectile(int points)
		: hasTouchedTarget_(false)
		, points_(points)
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

	int Projectile::getPoints() const
	{
		return points_;
	}

	Component::Category Projectile::getCategory() const
	{
		return Component::Projectile;
	}


} // namespace ecs
