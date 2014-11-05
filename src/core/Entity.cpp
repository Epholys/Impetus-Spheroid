#include "core/Entity.hpp"

//-----------------------------------------------------------------------------

Entity::Entity(ecs::EntityManager& entm, EntityID::Type type)
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

ecs::EntityManager::componentTable
Entity::getComponents()
{
	return ecs_.getAllComponents(label_);
}

void Entity::addModifier(Modifier<Entity> modifier)
{
	modifiers_.push_back(modifier);
}

void Entity::update(Time dt)
{
	for(auto& modifier : modifiers_)
	{
		modifier(*this, dt);
	}
						
	auto itEnd = std::remove_if(modifiers_.begin(),
	                            modifiers_.end(),
	                            [](Modifier<Entity>& modEnt)
	                            {
		                            return modEnt.isExpired();
	                            });
	
	modifiers_.erase(itEnd, modifiers_.end());
}

void Entity::draw(sf::RenderTarget&, sf::RenderStates) const
{
}


