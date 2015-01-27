#ifndef DIFFICULTY_MANAGER_HPP
#define DIFFICULTY_MANAGER_HPP


#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"
#include "data/DifficultyData.hpp"

class World;
namespace evt {
	class EventGenerator;
}

struct DifficultyContext
{
	World* world;
	evt::EventGenerator* eventGenerator;
};



class DifficultyManager : public sf::Drawable
{
public:
	DifficultyManager(DifficultyContext context);
	~DifficultyManager();

	void update(Time dt);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	template<typename T>
	auto selectDataIndex(std::vector<T> v);

	void updateDifficulty();

private:
	const Time phaseDuration_;

private:
	Time phaseTime_;
	unsigned int phaseNumber_;

	DifficultyContext context_;

	sf::Font font_;
	sf::Text timer_;
};

#include "core/DifficultyManager.ipp"


#endif // DIFFICULTY_MANAGER_HPP
