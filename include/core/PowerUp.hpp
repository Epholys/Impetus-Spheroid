#ifndef POWER_UP_HPP
#define POWER_UP_HPP

#include <string>

#include "core/Identifiers.hpp"

class World;

class PowerUp
{
public:
	PowerUp();
	virtual ~PowerUp();

 	virtual void apply (World& world) =0;

	virtual PowerUpType::Type getType() const =0;
};


#endif // POWER_UP_HPP
