#ifndef WALL_HPP
#define WALL_HPP

#include <SFML/Graphics/RectangleShape.hpp>

#include "core/Entity.hpp"

class Wall : public Entity
{
public:
	Wall(ecs::EntityManager& entm,
	     Vector2f position,
	     Vector2f size,
	     sf::Color color);
	virtual ~Wall();

	virtual void update(Time dt = Time::Zero);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::RectangleShape rect_;
};


#endif // WALL_HPP
