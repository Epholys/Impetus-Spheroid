#ifndef POWER_UP_HPP
#define POWER_UP_HPP

#include <string>

class World;

namespace PowerUpType
{
 	enum Type
 	{
 		None = 0,
 		Modifier,
 		Toogle,
 	};
}

namespace PowerUpID
{
	enum ID
	{
		None = -1,
		GhostBall,
		NoGravBall,
		CancelEvents,
		AddTime,
		AddTarget,
		PointMultiplier,
		BallTouchDouble,
		AddCannon,
		PowerUpCount,
	};
}

class PowerUp
{
public:
	PowerUp();
	virtual ~PowerUp();

 	virtual void apply (World& world) =0;

	virtual PowerUpType::Type getType() const =0;
};


#endif // POWER_UP_HPP
