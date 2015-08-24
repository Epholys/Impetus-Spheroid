#ifndef BALL_HPP
#define BALL_HPP


#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "utility/utility.hpp"
#include "utility/random.hpp"
#include "core/Entity.hpp"
#include "data/BallData.hpp"


/* Concrete derived class representing a Ball.
 *
 * */

class Ball : public Entity
{
public:
	enum Type : unsigned int
	{
		Normal = 0,
		Massless = 1,
		Ghost = 1 << 1,
	};

public:
	Ball(World& world,
	     Vector2f position,
	     Vector2f gravVect,
	     BallData data,
	     int nCollisionTarget = 1,
	     unsigned int type = 0);
	virtual ~Ball();

	virtual void update(Time dt);
	virtual void draw(sf::RenderTarget& target,
	                  sf::RenderStates states) const;

	sf::Color getColor() const;
	void setOutlineColor(sf::Color color);
	void reverseOutlineColor();

private:
	/* This function is a dirty hack: it creates ParticleEmitters via Emitter
	 * with just the correct rate to create a single particle during the
	 * Emitter::update() call inside this function. The ParticleEmitters are
	 * then destroyed. 
	 * */
	void makeMicroParticles(Time dt, std::shared_ptr<ecs::Projectile> projectileComponent);
	
public:
	static const float RADIUS_;
	static const float MASS_;

protected:
	sf::CircleShape ball_;
	Vector2f* position_;
	int trailEmitter_;
};

#endif // BALL_HPP
