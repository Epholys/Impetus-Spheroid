#include "core/PowerUpToogle.hpp"
#include "core/World.hpp"

//-----------------------------------------------------------------------------

PowerUpToogle::PowerUpToogle()
	: PowerUp()
	, activate_()
	, deactivate_()
	, isActivated_(false)

{
}

PowerUpToogle::~PowerUpToogle()
{
}


//-----------------------------------------------------------------------------

PowerUpType::Type PowerUpToogle::getType() const
{
 	return PowerUpType::Toogle;
}

void PowerUpToogle::apply (World& world)
{
	if(!isActivated_ && activate_)
	{
		activate_(world);
		isActivated_ = true;
	}
	else if(isActivated_ && deactivate_)
	{
		deactivate_(world);
		isActivated_ = false;
	}
}


//-----------------------------------------------------------------------------

void PowerUpToogle::addActivateFunc(std::function<void(World&)> func)
{
	activate_ = func;
}

void PowerUpToogle::addDeactivateFunc(std::function<void(World&)> func)
{
	deactivate_ = func;
}

bool PowerUpToogle::isActivated() const
{
	return isActivated_;
}
