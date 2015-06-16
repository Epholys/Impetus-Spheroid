#include "core/World.hpp"
#include "core/Obstacle.hpp"


//-----------------------------------------------------------------------------

namespace
{
	const float MASS = 10000.f;
	const sf::Color COLOR = sf::Color::Black;
}


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Obstacle::Obstacle(World& world,
                   Vector2f position,
                   Vector2f size,
                   Vector2f velocity,
                   Vector2f gravVect)
	: Entity(world, world.getEntityManager(), EntityID::Obstacle)
	, rect_(size)
{
	label_ = ecs::createObstacle(ecs_, position, size, velocity, MASS, gravVect);
	
	centerOrigin(rect_);
	rect_.setFillColor(COLOR);

	update(Time());

}

Obstacle::~Obstacle()
{
}

//-----------------------------------------------------------------------------
// *** virtual functions: ***


void Obstacle::update(Time dt)
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

void Obstacle::draw(sf::RenderTarget& target,
                  sf::RenderStates states) const
{
	Entity::draw(target, states);

	states.transform *= getTransform();

	target.draw(rect_, states);
}
