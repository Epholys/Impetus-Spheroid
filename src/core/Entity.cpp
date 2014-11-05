#include "core/Entity.hpp"

//-----------------------------------------------------------------------------

Entity::Entity(ecs::EntityManager& entm, Entity::Type type)
	: ecs_(entm)
	, label_(0)
	, type_(type)
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

EntityID::Type Entity::getType() const
{
	return type_;
}

void Entity::update(Time)
{
}

void Entity::draw(sf::RenderTarget&, sf::RenderStates) const
{
}


