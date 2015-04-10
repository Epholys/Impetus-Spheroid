#include "core/PowerUp.hpp"
#include "core/PowerUpModifier.hpp"
#include "core/World.hpp"

//-----------------------------------------------------------------------------

PowerUpModifier::PowerUpModifier()
	: PowerUp()
	, modifier_()
{
}

PowerUpModifier::~PowerUpModifier()
{
}


//-----------------------------------------------------------------------------

PowerUpType::Type PowerUpModifier::getType() const
{
	return PowerUpType::Modifier;
}

void PowerUpModifier::apply (World& world)
{
	world.addModifier(modifier_);
}


//-----------------------------------------------------------------------------

void PowerUpModifier::addModifier(Modifier<World> modifier)
{
	modifier_ = modifier;
}
