#include "core/Wall.hpp"
#include "framework/Assertion.hpp"


//-----------------------------------------------------------------------------

Wall::Wall(ecs::EntityManager& entm,
           Vector2f position,
           Vector2f size,
           sf::Color color)
	: Entity(entm, EntityID::Wall)
	, rect_(size)
{
	label_ = ecs::createWall(entm, position, size);

	rect_.setFillColor(color);
	auto rectBounds = rect_.getLocalBounds();
	rect_.setOrigin(rectBounds.left + rectBounds.width / 2.f,
	                rectBounds.top + rectBounds.height / 2.f);

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
	assert(pointPos);
	auto position = pointPos->position_;
	setPosition(position.x, position.y);
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(rect_, states);
}
