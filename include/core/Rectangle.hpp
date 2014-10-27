#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML/Graphics/RectangleShape.hpp>

#include "core/Entity.hpp"

class Rectangle : public Entity
{
public:
	Rectangle(ecs::EntityManager& entm,
	          Vector2f position,
	          Vector2f size,
	          sf::Color color);
	virtual ~Rectangle();

	virtual void update(Time dt = Time::Zero);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

protected:
	sf::RectangleShape rect_;
};


#endif // RECTANGLE_HPP
