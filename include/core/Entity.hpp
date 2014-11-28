#ifndef ENTITY_HPP
#define ENTITY_HPP


#include <memory>
#include <vector>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "framework/Assertion.hpp"
#include "utility/utility.hpp"
#include "utility/Time.hpp"
#include "ecs/EntityManager.hpp"
#include "ecs/Archetype.hpp"
#include "core/Identifiers.hpp"
#include "core/Modifier.hpp"

class World;

class Entity : public sf::Drawable, public sf::Transformable
{
public:
	typedef std::unique_ptr<Entity> Ptr;

public:
	Entity(World* world, ecs::EntityManager& entm, EntityID::Type type = EntityID::None);
	virtual ~Entity();

	ecs::Entity getLabel() const;
	EntityID::Type getType() const;
	
	ecs::EntityManager::componentTable getComponents();

	void addModifier(Modifier<Entity> modifier);
	
	virtual void update(Time dt) =0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
protected:
	World* world_;

	ecs::EntityManager& ecs_;
	ecs::Entity label_;
	
	EntityID::Type type_;

	std::vector<Modifier<Entity>> modifiers_;
};


#endif // ENTITY_HPP
