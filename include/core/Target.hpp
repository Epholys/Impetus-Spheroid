#ifndef TARGET_HPP
#define TARGET_HPP


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utility/utility.hpp"
#include "core/Entity.hpp"

class Target : public Entity
{
public:
	Target(World* world,
		   ecs::EntityManager& entm,
	       Vector2f position,
	       Vector2f size,
	       sf::Color color);
	virtual ~Target();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

	void updateColor();

private:
	void updateObjective();
	void moveToObjective();

protected:
	sf::RectangleShape rect_;

	Vector2f objective_;
};


#endif // TARGET_HPP
