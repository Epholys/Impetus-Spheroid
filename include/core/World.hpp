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
#include "core/EventGenerator.hpp"
#include "core/DifficultyManager.hpp"
#include "data/DifficultyData.hpp"

class World : public sf::NonCopyable
{
public:
	World(ecs::EntityManager& ecs, sf::RenderWindow& window, int precision = 4);

	void handleInput();
	void update(Time dt);
	void draw() const;

	ecs::EntityManager& getEntityManager();
	Vector2f& getGravityVect();
	const eg::PhysicEngine& getPhysicEngine() const;
	Vector2u getWindowSize() const;

	void addEntityModifier(Modifier<Entity> modifier);
	void addModifier(Modifier<World> modifier);
	void addEntity(Entity::Ptr entity);

	void updateDifficulty(DifficultyWorld diff);

private:
	void generateWorld();

	void getEvent(Time dt);
	void applyModifiers(Time dt);

	void cleanEntities();
	void cleanModifiers();

// TODO: Replace all the input by a separate class
	void createBall(Vector2f mousePosition);
	void createTarget(Vector2f mousePosition);

private:
	ecs::EntityManager& ecs_;
	sf::RenderWindow& window_;
	eg::PhysicEngine physEng_;
	evt::EventGenerator evtGen_;
	DifficultyManager difficulty_;

	float speedCoeff_;

	std::vector<Entity::Ptr> entities_;
	std::vector<Modifier<Entity>> entitiesModifiers_;
	unsigned int ballType_;
	Vector2f gravityVect_;

	std::vector<Modifier<World>> modifiers_;
	std::vector<Modifier<World>> modifierBuffer_;

	sf::Font font_;
	int score_;
	sf::Text scoreText_;
	
	// Temporary attributes to shift from Application to World
	float ballMass_;
	float ballRadius_;
	sf::Color ballColor_;
};


#endif // WORLD_HPP
