#include "utility/random.hpp"
#include "core/Ball.hpp"



//-----------------------------------------------------------------------------

Ball::Ball(World* world,
           ecs::EntityManager& entm,
           Vector2f position, 
           float radius, 
           float mass, 
           Vector2f gravVect,
           unsigned int type)
	: Entity(world, entm, EntityID::Ball)
	, ball_(radius)
{
	label_ = ecs::createBall(entm, position, radius, mass, gravVect);

	if(type & Massless)
	{
		entm.removeComponent(label_, ecs::Component::Mass);
	}
	if(type & Ghost)
	{
		entm.removeComponent(label_, ecs::Component::Solid);
	}

	auto ballRect = ball_.getLocalBounds();
	ball_.setOrigin(ballRect.left + ballRect.width / 2.f,
	                ballRect.top + ballRect.height / 2.f);
	ball_.setFillColor(sf::Color::Red);

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
		setPosition(position.x, position.y);
	}
}

void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	Entity::draw(target, states);
	
	states.transform *= getTransform();

	target.draw(ball_, states);
}
