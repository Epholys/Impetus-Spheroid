#include "core/Target.hpp"


//-----------------------------------------------------------------------------
// *** constructor and destructor: ***

Target::Target(ecs::EntityManager& entm,
               Vector2f position,
               Vector2f size,
               sf::Color color)
	: Entity(entm, EntityID::Target)
	, rect_(size)
{
	label_ = ecs::createTarget(entm, position, size);

	rect_.setFillColor(color);
	auto rectBounds = rect_.getLocalBounds();
	rect_.setOrigin(rectBounds.left + rectBounds.width / 2.f,
	                rectBounds.top + rectBounds.height / 2.f);

	update(Time());
}

Target::~Target()
{
}

//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Target::update(Time dt)
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

void Target::draw(sf::RenderTarget& target,
                  sf::RenderStates states) const
{
	Entity::draw(target, states);

	states.transform *= getTransform();

	target.draw(rect_, states);
}
