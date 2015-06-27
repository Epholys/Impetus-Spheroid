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
	, position_(nullptr)
{
	label_ = ecs::createWall(ecs_, position, size);

	centerOrigin(rect_);
	rect_.setFillColor(color);
	rect_.setOutlineThickness(-3.f);
	rect_.setOutlineColor(color);
	update(Time());

	auto pointPos = dynCast<ecs::Position>
		(ecs_.getComponent(label_, ecs::Component::Position));
	if(pointPos)
	{
		position_ = &pointPos->position_;
	}
}

Wall::~Wall()
{
}


//-----------------------------------------------------------------------------
// *** virtual functions: ***

void Wall::update(Time dt)
{
	Entity::update(dt);

	if(position_)
	{
		// NOT rect_.setPosition(...): rect__'s Transformable base class isn't
		// used because if suddenly Wall's view become several sf::Sprites and
		// sf::Shapes, it would mean I'll have to update every single one of them.
		setPosition(position_->x, position_->y);
	}
}

void Wall::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	
	target.draw(rect_, states);
}


//-----------------------------------------------------------------------------

Vector2f Wall::getPosition() const
{
	return *position_;
}

Vector2f Wall::getSize() const
{
	return Vector2f(rect_.getSize());
}

void Wall::setOutlineColor(sf::Color color)
{
	rect_.setOutlineColor(color);
}

void Wall::resetOutlineColor()
{
	rect_.setOutlineColor(rect_.getFillColor());
}


