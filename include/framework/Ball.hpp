
// TEMPORARY TEST CLASS

#ifndef BALL_HPP
#define BALL_HPP


#include <SFML/Graphics.hpp>

#include "utility/Vector2.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/Archetype.hpp"

class Ball : public sf::Drawable
{
public:
	Ball(ecs::EntityManager& entm, Vector2f position, float radius, float massg);
	virtual ~Ball();

	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	ecs::Entity getLabel() const;

private:
	ecs::EntityManager& ecs_;
	ecs::Entity label_;

	sf::CircleShape ball_;
};


#endif // BALL_HPP
