#ifndef DIFFICULTY_MANAGER_HPP
#define DIFFICULTY_MANAGER_HPP


#include <algorithm>
#include <map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"
#include "data/DifficultyData.hpp"
#include "gui/Menu.hpp"

class World;
namespace evt {
	class EventGenerator;
}
class Inventory;
class MetaData;

struct DifficultyContext
{
	World* world;
	evt::EventGenerator* eventGenerator;
	MetaData* metaData;
	Inventory* inventory;
};



class DifficultyManager : public sf::Drawable
{
public:
	DifficultyManager(DifficultyContext context);
	~DifficultyManager();

	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void handleInput(const sf::Event& event);

	void addTime(Time adding);
	
	void reset();
	void mask();
	
private:
	void createGui();

	template<typename T>
	auto selectDataIndex(std::vector<T> v);

	void updateDifficulty();
	void updateScore();
	void updateObjective();

// Temp functions for prototyping
	void reloadDifficulty();

private:
	Time phaseDuration_;
	static const int baseCeiling_ = 20;

private:
	Time phaseTime_;
	unsigned int phaseNumber_;

	DifficultyContext context_;

	sf::Font font_;
	sf::Text timer_;

	float score_;
	int objective_;
	int objectiveIncrement_;
	int ceiling_;
	sf::Text scoreText_;
	std::map<int, int> ballCount_;
	
	gui::Component::SPtr diffGui_;
	bool maskGui_;


// Temp attributes for quick adjustements
private:
	DifficultyWorldSeed worldSeed_;
	DifficultyEventGenSeed eventSeed_;
};

#include "core/DifficultyManager.ipp"


#endif // DIFFICULTY_MANAGER_HPP
