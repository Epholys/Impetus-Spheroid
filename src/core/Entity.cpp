#include "core/Entity.hpp"

//-----------------------------------------------------------------------------

Entity::Entity(ecs::EntityManager& entm)
	: ecs_(entm)
	, label_(0)
{
}

Entity::~Entity()
{
	ecs_.removeEntity(label_);
}

ecs::Entity Entity::getLabel() const
{
	return label_;
}

void Entity::update(Time)
{
}

void Entity::draw(sf::RenderTarget&, sf::RenderStates) const
{
}


