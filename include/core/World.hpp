#ifndef WORLD_HPP
#define WORLD_HPP


#include <vector>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "utility/Time.hpp"
#include "utility/utility.hpp"
#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
#include "core/Entity.hpp"
#include "core/Wall.hpp"
#include "core/Ball.hpp"

class World : public sf::NonCopyable
{
public:
	World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision = 4);

	void handleInput(sf::Event event);
	void update(Time dt);
	void draw() const;

private:
	void generateWorld();

	void cleanEntities();

private:
	ecs::EntityManager& ecs_;
	sf::RenderWindow& window_;
	eg::PhysicEngine physEng_;

	std::vector<Entity::Ptr> entities_;
};


#endif // WORLD_HPP
