#ifndef BALL_HPP
#define BALL_HPP


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utility/utility.hpp"
#include "core/Entity.hpp"
#include "data/BallData.hpp"


/* Concrete derived class representing a Ball.
 *
 * */

class Ball : public Entity
{
public:
	enum Type : unsigned int
	{
		Normal = 0,
		Massless = 1,
		Ghost = 1 << 1,
	};

public:
	Ball(World& world,
	     Vector2f position,
	     Vector2f gravVect,
	     BallData data,
	     int nCollisionTarget = 1,
	     unsigned int type = 0);
	virtual ~Ball();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

public:
	static const float RADIUS_;
	static const float MASS_;

protected:
	sf::CircleShape ball_;
};

#endif // BALL_HPP
