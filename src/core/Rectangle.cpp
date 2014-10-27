#include "core/Rectangle.hpp"
#include "framework/Assertion.hpp"


//-----------------------------------------------------------------------------

Rectangle::Rectangle(ecs::EntityManager& entm,
                     Vector2f position,
                     Vector2f size,
                     sf::Color color)
	: Entity(entm)
	, rect_(size)
{
	label_ = ecs::createRect(entm, position, size);

	rect_.setFillColor(color);
	auto rectBounds = rect_.getLocalBounds();
	rect_.setOrigin(rectBounds.left + rectBounds.width / 2.f,
	                rectBounds.top + rectBounds.height / 2.f);

	update();
}

Rectangle::~Rectangle()
{
}


//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Rectangle::update(Time)
{
	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	assert(pointPos);
	auto position = pointPos->position_;
	setPosition(position.x, position.y);
}

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(rect_, states);
}
