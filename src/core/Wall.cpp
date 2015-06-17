#include "core/Wall.hpp"
#include "core/World.hpp"
#include "framework/Assertion.hpp"


//-----------------------------------------------------------------------------

Wall::Wall(World& world,
           Vector2f position,
           Vector2f size,
           sf::Color color)
	: Entity(world, world.getEntityManager(), EntityType::Wall)
	, rect_(size)
{
	label_ = ecs::createWall(ecs_, position, size);

	centerOrigin(rect_);
	rect_.setFillColor(color);
	update(Time());
}

Wall::~Wall()
{
}


//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Wall::update(Time dt)
{
	Entity::update(dt);

	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	if(pointPos)
	{
		auto position = pointPos->position_;

		// NOT rect_.setPosition(...): rect__'s Transformable base class isn't
		// used because if suddenly Wall's view become several sf::Sprites and
		// sf::Shapes, it would mean I'll have to update every single one of them.
		setPosition(position.x, position.y);
	}
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(rect_, states);
}
