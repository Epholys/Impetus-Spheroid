#ifndef CANNON_HPP
#define CANNON_HPP

#include <deque>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Event.hpp>

#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "utility/utility.hpp"
#include "ecs/Entity.hpp"
#include "ecs/ComponentPhysic.hpp"
#include "core/Modifiable.hpp"
#include "core/Ball.hpp"
#include "data/BallData.hpp"


class World;
class Inventory;

/* Manages all the ball shooting: if (absurd coordinates or not?), when
 * (auto-fire mode), where, ball's type & inventory's power-ups
 * consumming... But also the View with the cannon itself and the preview of the
 * next balls.
 * */

class Cannon : 
	public sf::Drawable,
	public Modifiable<Cannon>
{
public:
	Cannon(const Vector2f& position, World& world, Inventory& inventory);
	~Cannon();

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void updateTubeDirection();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void switchBallType(unsigned int type);
	void setNTouching(int nTouching);

	// WARNING: returns -1 if no Ball is created.
	ecs::Entity createBall();

private:
	void initView();

	void applyAutoFire(Time dt);
	void updateInventory();
	void applyBallType();
	BallData randomBallData() const;

	void drawFutureBalls(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	World& world_;
	Inventory& inventory_;

	Vector2f position_;

	Time untilNextFire_;
	Time timeBeetweenFire_;

	unsigned int ballType_;
	int nTouchingBall_;

	std::deque<std::pair<BallData, unsigned int>> ballBuffer_;

	sf::CircleShape cannonBody_;
	sf::RectangleShape cannonTube_;
};


#endif // CANNON_HPP


