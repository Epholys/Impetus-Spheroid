#ifndef WORLD_HPP
#define WORLD_HPP


#include <sstream>
#include <vector>
#include <algorithm>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"
#include "utility/utility.hpp"
#include "ecs/EntityManager.hpp"
#include "engine/PhysicEngine.hpp"
#include "core/Entity.hpp"
#include "core/Wall.hpp"
#include "core/Ball.hpp"
#include "core/Target.hpp"
#include "core/Modifier.hpp"

class World : public sf::NonCopyable
{
public:
	World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision = 4);

	void handleInput();
	void update(Time dt);
	void draw() const;

private:
	void generateWorld();

	void applyModifiers(Time dt);

	void cleanEntities();
	void cleanModifiers();

private:
	ecs::EntityManager& ecs_;
	sf::RenderWindow& window_;
	eg::PhysicEngine physEng_;

	std::vector<Entity::Ptr> entities_;
	std::vector<Modifier<Entity>> entitiesModifiers_;
	unsigned int ballType_;

	std::vector<Modifier<World>> modifiers_;
	
	// Temporaries attributes to shift from Application to World
	sf::Font font_;
	int score_;
	sf::Text scoreText_;

	float ballMass_;
	float ballRadius_;
	sf::Color ballColor_;
};


#endif // WORLD_HPP
