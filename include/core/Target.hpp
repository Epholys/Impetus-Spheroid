#ifndef TARGET_HPP
#define TARGET_HPP


#include "utility/utility.hpp"
#include "core/Entity.hpp"

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

class Target : public Entity
{
public:
	Target(ecs::EntityManager& entm,
	       Vector2f position,
	       Vector2f size,
	       sf::Color color);
	virtual ~Target();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

protected:
	sf::RectangleShape rect_;
};


#endif // TARGET_HPP
