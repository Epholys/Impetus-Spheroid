#include "core/Entity.hpp"

//-----------------------------------------------------------------------------

Entity::Entity(World& world, ecs::EntityManager& entm, EntityType::Type type)
	: world_(world)
	, ecs_(entm)
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

EntityType::Type Entity::getType() const
{
	return type_;
}

ecs::EntityManager::componentTable
Entity::getComponents(bool force)
{
	return ecs_.getAllComponents(label_, force);
}

void Entity::update(Time dt)
{
	applyModifiers(*this, dt);
	cleanModifiers();
}

void Entity::draw(sf::RenderTarget&, sf::RenderStates) const
{
}


