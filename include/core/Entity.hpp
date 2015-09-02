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
#include "core/Modifier.hpp"
#include "core/Modifiable.hpp"
#include "core/Emitter.hpp"
#include "core/Transitionnable.hpp"


class World;

namespace EntityType
{
	enum Type  : unsigned int
	{
		None = 0,
		Wall = 1,
		Ball = 1 << 1,
		Target = 1 << 2,
		Obstacle = 1 << 3,
	};
}

/* Base abstract class defining an element defined in the ecs_ system by
 * ecs::Components linked with the tag label_ and coordinated by *world_. The
 * derived class are identified by type_ defined above.
 *
 * This class principally coordinate its View with its Model (which live in the
 * ecs_)
 * 
 * NOTE: Maybe refactor to use typeid() instead of type_.
 *
 * */ 
class Entity : public sf::Drawable, public sf::Transformable,
               public Modifiable<Entity>,
               public Emitter,
               public Transitionnable
{
public:
	typedef std::unique_ptr<Entity> Ptr;

public:
	Entity(World& world, ecs::EntityManager& entm, EntityType::Type type = EntityType::None);
	virtual ~Entity();

	ecs::Entity getLabel() const;
	EntityType::Type getType() const;
	World& getWorld();
	
	ecs::EntityManager::componentTable getComponents(bool force=false);

	virtual void update(Time dt) =0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	
protected:
	World& world_;

	ecs::EntityManager& ecs_;
	ecs::Entity label_;
	
	EntityType::Type type_;
};


#endif // ENTITY_HPP
