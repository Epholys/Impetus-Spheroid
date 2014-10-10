///////////////////////////////////////////////////////////////////////////////
// TEMPORARY TEST CLASS
///////////////////////////////////////////////////////////////////////////////

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <SFML/Graphics.hpp>

#include "utility/Vector2.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/Archetype.hpp"

class Rectangle : public sf::Drawable
{
public:
	Rectangle(ecs::EntityManager& entm, Vector2f position, Vector2f size, sf::Color color);
	virtual ~Rectangle();

	void update(Time dt = Time::Zero);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	ecs::Entity getLabel() const;

private:
	ecs::EntityManager& ecs_;
	ecs::Entity label_;
	
	sf::RectangleShape rect_;
};


#endif // RECTANGLE_HPP

///////////////////////////////////////////////////////////////////////////////
// TEMPORARY TEST CLASS
///////////////////////////////////////////////////////////////////////////////
