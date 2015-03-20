#include "core/PowerUpModifier.hpp"
#include "core/World.hpp"

//-----------------------------------------------------------------------------

PowerUpModifier::PowerUpModifier()
	: modifier_()
{
}

PowerUpModifier::~PowerUpModifier()
{
}


//-----------------------------------------------------------------------------

PowerUp::Type PowerUpModifier::getType() const
{
	return PowerUp::Modifier;
}

void PowerUpModifier::operator() (World& world)
{
	world.addModifier(modifier_);
}


//-----------------------------------------------------------------------------

void PowerUpModifier::addModifier(Modifier<World> modifier)
{
	modifier_ = modifier;
}
