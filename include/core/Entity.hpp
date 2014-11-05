#ifndef ENTITY_HPP
#define ENTITY_HPP


#include <memory>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "framework/Assertion.hpp"
#include "utility/utility.hpp"
#include "utility/Time.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/Archetype.hpp"
#include "core/Identifiers.hpp"

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	typedef std::unique_ptr<Entity> Ptr;

public:
	Entity(ecs::EntityManager& entm, EntityID::Type type = EntityID::None);
	virtual ~Entity();

	ecs::Entity getLabel() const;
	EntityID::Type getType() const;
	
	virtual void update(Time dt) =0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
protected:
	ecs::EntityManager& ecs_;
	ecs::Entity label_;
	
	Entity::Type type_;
};


#endif // ENTITY_HPP
