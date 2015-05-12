#ifndef WORLD_HPP
#define WORLD_HPP


#include <sstream>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

// Maybe I should make some generic header, like "core.hpp"
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
#include "core/Cannon.hpp"
#include "data/DifficultyData.hpp"
#include "data/BallData.hpp"

class Inventory;
class TransGamesData;

class World : public sf::NonCopyable,
              public sf::Drawable,
              public Modifiable<World>
{
public:
	enum GameState
	{
		Waiting,
		Playing,
		GameOver
	};

public:
	World(const Vector2f& originalSize,
	      const sf::Transform& globalTransform,
	      TransGamesData& datas,
	      int precision = 2);
	~World() {};

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawFutureBalls(sf::RenderTarget& target, sf::RenderStates states) const;

	ecs::EntityManager& getEntityManager();
	Vector2f& getGravityVect();
	Vector2u getWindowSize() const;
	const std::vector<eg::PhysicEngine::entityPair>& getTrackedCollisions() const;

	void addEntity(Entity::Ptr entity);
	template<typename T, typename... Args>
	void addEntity(Args... args);
	void removeEntity(ecs::Entity label);
	void addEntityModifier(Modifier<Entity> modifier);
	
	bool isGameOver() const;
	void setState(GameState state);

	void switchBallType(unsigned int type);
	void cancelEvents(bool comeFromInventory);
	void addTime(Time adding);
	void setNTouching(int n);

	void updateDifficulty(DifficultyWorld diff);

	ecs::Entity createBall();
	ecs::Entity createTarget(Vector2f mousePosition);

private:
	void generateWorld();

	void getEvent(Time dt);
	void applyAutoFire(Time dt);
	virtual void applyModifiers(Time dt);
	virtual void cleanModifiers();

	void cleanEntities();

// TODO: Replace all the input by a separate class
	void applyBallType();
	BallData genBallData() const;

private:
	const Vector2f originalSize_;
	const sf::Transform& globalTransform_;
	ecs::EntityManager ecs_;
	eg::PhysicEngine physEng_;
	evt::EventGenerator evtGen_;
	DifficultyManager difficulty_;
	Inventory& inventory_;
	Cannon cannon_;
	static Vector2f CANON_POSITION;

	GameState state_;

	Time untilNextFire_;
	static const Time TIME_BEETWEEN_FIRE;
	Vector2f mousePosition_;

	float speedCoeff_;

	std::vector<Entity::Ptr> entities_;
	std::vector<Modifier<Entity>> entitiesModifiers_;
	unsigned int ballType_;
	int nTouchingBall_;
	Vector2f gravityVect_;
	std::deque<std::pair<BallData, unsigned int>> ballBuffer_;

	// Temporary attributes to shift from Application to World
	float ballMass_;
	float ballRadius_;
};

#include "World.ipp"


#endif // WORLD_HPP
