#ifndef DIFFICULTY_MANAGER_HPP
#define DIFFICULTY_MANAGER_HPP


#include <algorithm>
#include <map>

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"
#include "framework/ResourceIdentifiers.hpp"
#include "data/DifficultyData.hpp"
#include "gui/Menu.hpp"
#include "gui/TransitionDeque.hpp"
#include "gui/CircleGauge.hpp"
#include "gui/ScoreGauge.hpp"

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


/* DifficultyManager schedules the increase of difficulty in world,
 * eventGenerator, and of the objective_ by separating the game into different
 * phases of PHASE_TIME_. It also managed all point counting.
 *
 * NOTE:
 *  - Could be separated into several small classes.n
 *  - The commented code used to display a GUI used to quickly modify difficulty
 * parameters or is linked to the old point system.
 *
 * */

class DifficultyManager : public sf::Drawable
{
public:
	DifficultyManager(DifficultyContext context);
	~DifficultyManager();

	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
//	void handleInput(const sf::Event& event);

	int getObjective() const;

	void addTime(Time adding);

private:
//	void createGui();

	template<typename T>
	auto selectDataIndex(std::vector<T> v);

	void updateDifficulty();
	void updateScore();
	void updateObjective();
	void updateIndicatorDuration();
	void updateIndicator(int points);

	sf::Color findColor(int points);

public:
	static const int BASE_OBJECTIVE_;
	static const Time PHASE_TIME_;

private:
	Time phaseTime_;
	unsigned int phaseNumber_;

	DifficultyContext context_;

	sf::Font& font_;
	gui::CircleGauge timerGauge_;

	float score_;
	int objective_;
	int ceiling_;
	sf::Text scoreText_;
	gui::ScoreGauge scoreGauge_;
	int coinsPerPointScore_;

	std::deque<sf::Text> indicatorTexts_;
	gui::TransitionDeque indicatorDeque_;
	Time indicatorAccumulator_;
	Time indicatorDuration_;
	

	// std::map<int, int> ballCount_;
	
	// gui::Component::SPtr diffGui_;
	// bool maskGui_;
};

#include "core/DifficultyManager.ipp"


#endif // DIFFICULTY_MANAGER_HPP
