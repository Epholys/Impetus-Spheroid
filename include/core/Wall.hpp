#ifndef WALL_HPP
#define WALL_HPP

#include <SFML/Graphics/RectangleShape.hpp>

#include "core/Entity.hpp"

/* Concrete derived class representing a Wall.
 *
 * */

class Wall : public Entity
{
public:
	Wall(World& world,
	     Vector2f position,
	     Vector2f size,
	     sf::Color color);
	virtual ~Wall();

	virtual void update(Time dt = Time::Zero);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::RectangleShape rect_;
	Vector2f* position_;
};


#endif // WALL_HPP
