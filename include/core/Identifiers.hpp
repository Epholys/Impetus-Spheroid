#ifndef IDENTIFIERS_HPP
#define IDENTIFIERS_HPP


namespace EntityID
{
	enum Type  : unsigned int
	{
		None = 0,
		Wall = 1,
		Ball = 1 << 1,
		Target = 1 << 2,
		Obstacle = 1 << 3,
	};
}


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
		None = 0,
		GhostBall,
		NoGravBall,
		CancelEvents,
		AutoFire,
		AddTime,
		AddTarget,
		PointMultiplier,
		BallTouchDouble,
	};
}

#endif // IDENTIFIERS_HPP
