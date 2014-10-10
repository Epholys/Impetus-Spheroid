///////////////////////////////////////////////////////////////////////////////
// TEMPORARY TEST CLASS
///////////////////////////////////////////////////////////////////////////////

#include "framework/Assertion.hpp"
#include "framework/Rectangle.hpp"
#include "utility/utility.hpp"

Rectangle::Rectangle(ecs::EntityManager& entm, Vector2f position, Vector2f size, sf::Color color)
	: ecs_(entm)
	, label_(0)
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
	ecs_.removeEntity(label_);
}

void Rectangle::update(Time)
{
	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	assert(pointPos);
	auto position = pointPos->position_;
	rect_.setPosition(position.x, position.y);
}

void Rectangle::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(rect_, states);
}

ecs::Entity Rectangle::getLabel() const
{
	return label_;
}

///////////////////////////////////////////////////////////////////////////////
// TEMPORARY TEST CLASS
///////////////////////////////////////////////////////////////////////////////

