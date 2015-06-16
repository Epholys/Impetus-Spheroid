#include "utility/random.hpp"
#include "core/Ball.hpp"
#include "core/World.hpp"

//-----------------------------------------------------------------------------

namespace
{
	const float RADIUS = 10.f;
	const float MASS = 1.f;
}


//-----------------------------------------------------------------------------

Ball::Ball(World& world,
           Vector2f position, 
           Vector2f gravVect,
           BallData data,
           int nCollisionTarget,
           unsigned int type)
	: Entity(world, world.getEntityManager(), EntityID::Ball)
	, ball_(RADIUS)
{
	label_ = ecs::createBall(ecs_, position, RADIUS, MASS, gravVect, data.point, nCollisionTarget);

	if(type & Massless)
	{
		ecs_.removeComponent(label_, ecs::Component::Mass);
	}
	if(type & Ghost)
	{
		ecs_.removeComponent(label_, ecs::Component::Solid);
	}

	centerOrigin(ball_);
	ball_.setFillColor(data.color);

	update(Time());
}

Ball::~Ball()
{
}


//-----------------------------------------------------------------------------
// *** Virtual functions: ***


void Ball::update(Time dt)
{
	Entity::update(dt);

	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	if(pointPos)
	{
		auto position = pointPos->position_;
		
		// NOT ball_.setPosition(...): ball_'s Transformable base class isn't
		// used because if suddenly Ball's view become several sf::Sprites and
		// sf::Shapes, it would mean I'll have to update every single one of them.
		setPosition(position.x, position.y);
	}
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	
	states.transform *= getTransform();

	target.draw(ball_, states);
}
