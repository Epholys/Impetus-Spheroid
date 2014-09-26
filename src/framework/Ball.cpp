#include <cassert>

#include "framework/Ball.hpp"
#include "utility/utility.hpp"


Ball::Ball(ecs::EntityManager& entm, Vector2f position, float radius)
	: ecs_(entm)
	, label_(0)
	, ball_(radius)
{
	label_ = ecs::createBall(entm, position, radius);

	ball_.setFillColor(sf::Color::Red);
	auto ballRect = ball_.getLocalBounds();
	ball_.setOrigin(ballRect.left + ballRect.width / 2.f,
	                ballRect.top + ballRect.height / 2.f);
}


Ball::~Ball()
{
	ecs_.removeEntity(label_);
}


void Ball::update(Time)
{
	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	assert(pointPos);
	auto position = pointPos->position_;
	ball_.setPosition(position.x, position.y);
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(ball_, states);
}

ecs::Entity Ball::getLabel() const
{
	return label_;
}
