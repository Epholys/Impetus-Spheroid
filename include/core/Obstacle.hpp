#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "core/Entity.hpp"

class Obstacle : public Entity
{
public:
	Obstacle(World& world,
	         Vector2f position,
	         Vector2f size,
		     Vector2f velocity,
	         Vector2f gravVect);
	virtual ~Obstacle();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

protected:
	sf::RectangleShape rect_;
	
};


#endif // OBSTACLE_HPP
