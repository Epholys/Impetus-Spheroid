#ifndef TARGET_HPP
#define TARGET_HPP


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utility/utility.hpp"
#include "core/Entity.hpp"

namespace TargetDefault
{
	const Vector2f SIZE {15.f, 75.f};
	const float RADIUS = 15.f;
	const sf::Color COLOR (255, 230, 0);
}

class Target : public Entity
{
public:
	Target(World& world,
	       Vector2f position);
	virtual ~Target();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

	Vector2f getSize() const;
	
	void updateColor();
	void changeSize(Vector2f newSize);
	void setShape(ecs::Collidable::Type shape);
	
private:
	void updateObjective();
	void moveToObjective();

protected:
	ecs::Collidable::Type shape_;
	sf::RectangleShape rect_;
	sf::CircleShape circ_;
	Vector2f* position_;

	// In this class, objective_ is the position where the Target have to go
	// when touched by a ecs::Projectile.
	Vector2f objective_;
};


#endif // TARGET_HPP
