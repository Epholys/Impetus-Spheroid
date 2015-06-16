#ifndef EVENT_GENERATOR_HPP
#define EVENT_GENERATOR_HPP


#include <algorithm>

#include "utility/random.hpp"
#include "data/DifficultyData.hpp"
#include "core/EventFunctions.hpp"
#include "core/Event.hpp"

namespace evt
{
	/* EventGenerator decide when and what Event World recieve (via update()).
	 *
	 * It is linked to DifficultyManager which send some information with
	 * updateDifficulty().
	 * */ 

	class EventGenerator
	{
	public:
		EventGenerator();

		Event update(Time dt);
		void updateDifficulty(DifficultyEventGen diff);

	private:
		void computeChanceSum();

		void updateChances(Event::Difficulty diff, unsigned int chance);

		void updateCounters(std::size_t nextEvt);
		std::size_t chooseEvent() const;
		

	private:
		std::vector<Event> events_;

		Time timeUntilNextEvent_;
		float timeBeetweenEvents_;

		unsigned int chanceSum_;
	};

}

#endif // EVENT_GENERATOR_HPP
