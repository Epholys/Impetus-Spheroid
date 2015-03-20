#ifndef POWER_UP_HPP
#define POWER_UP_HPP

#include "core/Modifier.hpp"

class World;

class PowerUp
{
public:
	enum Type
	{
		None = 0,
		Modifier,
		Toogle,
	};

public:
	PowerUp();
	virtual ~PowerUp();

	virtual void operator() (World& world) =0;

	virtual Type getType() const =0;
};


#endif // POWER_UP_HPP
