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
#include "core/Cannon.hpp"
#include "core/DifficultyManager.hpp"
#include "core/LastGameData.hpp"
#include "core/ParticleSystem.hpp"
#include "core/SunHighlight.hpp"
#include "data/DifficultyData.hpp"
#include "data/BallData.hpp"
#include "gui/MultiTransition.hpp"
#include "gui/utility.hpp"


class Inventory;
class MetaData;

class World : public sf::Drawable,
              public Modifiable<World>
{
public:
	enum GameState
	{
		WaitingTutorial,
		Waiting,
		Playing,
		GameOver
	};

public:
	World(const Vector2u& originalSize,
	      MetaData& metaData,
	      FontHolder& fonts,
	      TextureHolder& textures,
	      int precision = 1);
	~World() {};
	
	ecs::Entity createTarget(Vector2f mousePosition);
	unsigned int getNCannon() const;
	void addCannon();
	void removeCannon();

	void handleInput(const sf::Event& event);
	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	LastGameData getGameStats() const;

	ecs::EntityManager& getEntityManager();
	Vector2f getGravityVect() const;	
	void setGravityVect(Vector2f vect);
	ParticleSystem& getParticleSystem(Particle::Type type);
	Vector2u getWindowSize() const;
	const std::vector<eg::PhysicEngine::entityPair>& getTrackedCollisions() const;
	Vector2f getMousePosition() const;
	Wall* getCeiling();
	Wall* getLeftWall();
	sf::Font& getFontRef();

	void addEntity(Entity::Ptr entity);
	template<typename T, typename... Args>
	void addEntity(Args... args);
	void removeEntity(ecs::Entity label);

	// It's a trap: it's no really a generic function, all cases (3) are
	// specialized in the .cpp... but having a single interface is nicer.
	template<typename T>
	void forwardModifier(Modifier<T> modifier);

	void addSprite(TextureID::ID id, const std::string& path, sf::Color color, gui::MultiTransition transitions, bool fadeOut, bool isInFront);
	
	bool hasStarted() const;
	bool isGameOver() const;
	void setState(GameState state);

	void cancelEvents(bool comeFromInventory);
	void addTime(Time adding);

	void updateDifficulty(DifficultyWorld diff);

private:
	void generateWorld();

	void getEvent(Time dt);

	void cleanEntities();
	void cleanOtherDrawings();

private:
	const Vector2f originalSize_;
	sf::Font& font_;
	TextureHolder& textures_;

	const MetaData& metaData_;
	ecs::EntityManager ecs_;
	eg::PhysicEngine physEng_;
	evt::EventGenerator evtGen_;
	DifficultyManager difficulty_;
	Inventory& inventory_;
	std::vector<std::shared_ptr<Cannon>> cannons_;
	unsigned int nCannon_;

	GameState state_;

	Vector2f mousePosition_;

	float speedCoeff_;
	Vector2f gravityVect_;

	std::vector<Entity::Ptr> entities_;
	Wall* ceiling_;
	Wall* leftWall_;
	SunHighlight targetHighlight_;

	std::vector<ParticleSystem> particleSystems_;

	struct DrawingEntry
	{
		sf::Sprite sprite;
		gui::MultiTransition transitions;
		gui::FadeOut<sf::Sprite> fadeOut;
		bool isInFront;
	};
	std::vector<DrawingEntry> otherDrawings_;
};

#include "World.ipp"


#endif // WORLD_HPP
