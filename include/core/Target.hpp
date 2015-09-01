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
	enum Shape
	{
		Rect,
		Circle
	};

public:
	Target(World& world,
	       Vector2f position);
	virtual ~Target();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

	void updateColor();
	void changeSize(Vector2f newSize);
	void setShape(Shape shape);
	
private:
	void updateObjective();
	void moveToObjective();

protected:
	sf::RectangleShape rect_;
	Vector2f* position_;

	// In this class, objective_ is the position where the Target have to go
	// when touched by a ecs::Projectile.
	Vector2f objective_;
};


#endif // TARGET_HPP
