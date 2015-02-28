#ifndef WORLD_HPP
#define WORLD_HPP


#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"
#include "utility/utility.hpp"
#include "utility/random.hpp"
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
#include "data/BallData.hpp"

class World : public sf::NonCopyable
{
public:
	enum GameState
	{
		Waiting,
		Playing,
		GameOver
	};

public:
	World(sf::RenderWindow& window, int precision = 2);
	~World() {};

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void draw() const;
	void drawFutureBalls() const;

	ecs::EntityManager& getEntityManager();
	Vector2f& getGravityVect();
	Vector2u getWindowSize() const;
	const std::vector<eg::PhysicEngine::entityPair>& getTrackedCollisions() const;

	void addEntityModifier(Modifier<Entity> modifier);
	void addModifier(Modifier<World> modifier);
	void addEntity(Entity::Ptr entity);
	template<typename T, typename... Args>
	void addEntity(Args... args);

	bool isGameOver() const;
	void setState(GameState state);

	void updateDifficulty(DifficultyWorld diff);

private:
	void generateWorld();

	void getEvent(Time dt);
	void applyModifiers(Time dt);

	void cleanEntities();
	void cleanModifiers();

// TODO: Replace all the input by a separate class
	void createBall(Vector2f mousePosition);
	BallData genBallData() const;
	void createTarget(Vector2f mousePosition);

private:
	sf::RenderWindow& window_;
	ecs::EntityManager ecs_;
	eg::PhysicEngine physEng_;
	evt::EventGenerator evtGen_;
	DifficultyManager difficulty_;

	GameState state_;

	float speedCoeff_;

	std::vector<Entity::Ptr> entities_;
	std::vector<Modifier<Entity>> entitiesModifiers_;
	unsigned int ballType_;
	Vector2f gravityVect_;
	std::deque<BallData> ballBuffer_;

	std::vector<Modifier<World>> modifiers_;
	std::vector<Modifier<World>> modifierBuffer_;

	// Temporary attributes to shift from Application to World
	float ballMass_;
	float ballRadius_;
	sf::Color ballColor_;
};

#include "World.ipp"


#endif // WORLD_HPP
