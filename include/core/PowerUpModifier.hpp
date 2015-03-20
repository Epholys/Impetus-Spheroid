#ifndef POWER_UP_MODIFIER_HPP
#define POWER_UP_MODIFIER_HPP

#include "core/PowerUp.hpp"

class World;

class PowerUpModifier
{
public:
	PowerUpModifier();
	virtual ~PowerUpModifier();

	virtual void operator() (World& world);
	virtual PowerUp::Type getType() const;

	void addModifier(Modifier<World> modifier);

protected:
	Modifier<World> modifier_;
};


#endif // POWER_UP_MODIFIER_HPP
