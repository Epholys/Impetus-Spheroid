#ifndef CANNON_HPP
#define CANNON_HPP

#include <deque>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Window/Event.hpp>

#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "ecs/Entity.hpp"
#include "core/Modifiable.hpp"
#include "core/Ball.hpp"
#include "data/BallData.hpp"


class World;
class Inventory;

class Cannon : 
	public sf::Drawable,
	public Modifiable<Cannon>
{
public:
	Cannon(const Vector2f& position, World& world, Inventory& inventory);
	~Cannon();

	// NOTE: Cannon knows nothing about the Global Transform, etc.
	// It's World's job to make sure event if correctely adapted to the
	// originalWindow_ attribute.
	void handleInput(const sf::Event& event);
	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void switchBallType(unsigned int type);
	void setNTouching(int nTouching);
	ecs::Entity createBall();

private:
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

	float ballMass_;
	float ballRadius_;
	unsigned int ballType_;
	int nTouchingBall_;

	std::deque<std::pair<BallData, unsigned int>> ballBuffer_;
};


#endif // CANNON_HPP
