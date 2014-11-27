#ifndef BALL_HPP
#define BALL_HPP


#include "utility/utility.hpp"
#include "core/Entity.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

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
	Ball(ecs::EntityManager& entm,
	     Vector2f position,
	     float radius,
	     float mass,
	     sf::Color color,
	     Vector2f gravVect,
	     unsigned int type = 0);
	virtual ~Ball();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

protected:
	sf::CircleShape ball_;
};

#endif // BALL_HPP
