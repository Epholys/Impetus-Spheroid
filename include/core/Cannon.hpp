#ifndef CANNON_HPP
#define CANNON_HPP


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>

#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "ecs/Entity.hpp"
#include "core/Modifiable.hpp"

class Cannon : 
	public sf::Drawable, public sf::Transformable,
	public Modifiable<Cannon>
{
public:
	Cannon(const Vector2f& windowSize);
	~Cannon();

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	ecs::Entity createBall(Vector2f mousePosition);

private:
	Vector2f position_;
};


#endif // CANNON_HPP
