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

	void Target::setPointMultiplier(float pMul)
	{
		pointMultiplier_ = pMul;
	}

//-----------------------------------------------------------------------------
	// *** Projectile: ***

	Projectile::Projectile(int points, int nTouching)
		: hasTouchedTarget_(false)
		, nTouching_(nTouching)
		, points_(points)
	{
	}

	void Projectile::touchTarget()
	{
		--nTouching_;
		if(nTouching_ < 1)
		{
			hasTouchedTarget_ = true;
		}
	}

	bool Projectile::hasTouchedTarget() const
	{
		return hasTouchedTarget_;
	}

	int Projectile::getPoints() const
	{
		return points_;
	}

	int Projectile::getNTouching() const
	{
		return nTouching_;
	}

	Component::Category Projectile::getCategory() const
	{
		return Component::Projectile;
	}


} // namespace ecs
