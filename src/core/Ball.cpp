#include "core/Ball.hpp"



//-----------------------------------------------------------------------------

Ball::Ball(ecs::EntityManager& entm,
           Vector2f position, 
           float radius, 
           float mass, 
           sf::Color color)
	: Entity(entm)
	, ball_(radius)
{
	label_ = ecs::createBall(entm, position, radius, mass);

	ball_.setFillColor(color);
	auto ballRect = ball_.getLocalBounds();
	ball_.setOrigin(ballRect.left + ballRect.width / 2.f,
	                ballRect.top + ballRect.height / 2.f);
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
	assert(pointPos);
	auto position = pointPos->position_;
	setPosition(position.x, position.y);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	
	states.transform *= getTransform();

	target.draw(ball_, states);
}
