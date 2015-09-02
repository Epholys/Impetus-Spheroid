#ifndef CANNON_HPP
#define CANNON_HPP

#include <deque>
#include <vector>
#include <functional>

#include <SFML/Graphics.hpp>
#include <SFML/Window/Event.hpp>

#include "utility/Time.hpp"
#include "utility/Vector2.hpp"
#include "utility/utility.hpp"
#include "ecs/Entity.hpp"
#include "ecs/ComponentPhysic.hpp"
#include "core/Modifiable.hpp"
#include "core/Ball.hpp"
#include "core/PowerUp.hpp"
#include "data/BallData.hpp"
#include "gui/TransitionDeque.hpp"


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
	Cannon(const Vector2f& position, Vector2f ballPosition, World& world, Inventory& inventory, int ballsPerSecond);
	~Cannon();

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void updateTubeDirection();
	void updateArcPreview();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void switchBallType(unsigned int type);
	void setNTouching(int nTouching);
	void setShootingOrderFunction(std::function<Vector2f()> function);
	void setCreateBallExpansion(std::function<PowerUpID::ID(Entity*, BallData)> function);

	void modulateColor(sf::Color color = sf::Color::White);

	// WARNING: returns -1 if no Ball is created.
	ecs::Entity createBall();

private:
	void initView();

	void applyAutoFire(Time dt);
	void updateInventory();
	Vector2f computeBallVelocity(Vector2f mousePosition);
	void applyBallType();
	BallData randomBallData() const;

	std::vector<Vector2f> computeBallTrajectory();
	void computeArcView(const std::vector<Vector2f>& trajectory);
	
	void updateBuffer();

	std::function<Vector2f()> getShootingOrderPosition;
	std::function<PowerUpID::ID(Entity*, BallData data)> createBallExpansion;

private:
	World& world_;
	Inventory& inventory_;

	Vector2f position_;

	Time untilNextFire_;
	Time timeBeetweenFire_;

	unsigned int ballType_;
	int nTouchingBall_;

	struct BufferEntry
	{
		BallData data;
		unsigned int type;
		sf::CircleShape shape;
	};
	std::deque<BufferEntry> ballBuffer_;
	gui::TransitionDeque transitionDeque_;

	sf::CircleShape cannonBody_;
	sf::RectangleShape cannonTube_;
	sf::VertexArray arcPreview_;
};


#endif // CANNON_HPP
