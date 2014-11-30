#include "core/World.hpp"
#include "core/Obstacle.hpp"


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Obstacle:: Obstacle(World* world,
                    ecs::EntityManager& entm,
                    Vector2f position,
                    Vector2f size,
                    Vector2f velocity,
                    Vector2f gravVect,
                    sf::Color color)
	: Entity(world, entm, EntityID::Obstacle)
	, rect_(size)
{
	const float MASS = 1000.f;

	label_ = ecs::createObstacle(entm, position, size, velocity, MASS, gravVect);
	
	rect_.setFillColor(color);
	auto rectBounds = rect_.getLocalBounds();
	rect_.setOrigin(rectBounds.left + rectBounds.width / 2.f,
	                rectBounds.top + rectBounds.height / 2.f);

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
