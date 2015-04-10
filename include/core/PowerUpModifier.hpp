#ifndef POWER_UP_MODIFIER_HPP
#define POWER_UP_MODIFIER_HPP


#include "core/PowerUp.hpp"
#include "core/Modifier.hpp"

class World;

class PowerUpModifier : public PowerUp
{
public:
	PowerUpModifier();
	virtual ~PowerUpModifier();

	virtual void apply (World& world);
	virtual PowerUpType::Type getType() const;

	void addModifier(Modifier<World> modifier);

protected:
	Modifier<World> modifier_;
};


#endif // POWER_UP_MODIFIER_HPP
