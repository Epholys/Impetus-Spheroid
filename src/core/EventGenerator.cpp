#include "core/EventGenerator.hpp"


//-----------------------------------------------------------------------------

namespace
{
	const float BASE_TIME_BEETWEEN_EVENTS = 3.f;
	const float FIRST_EVENT_TIME = 5.f;
	const float FIRST_EVENT_TIME_DEV = 2.f;
}


//-----------------------------------------------------------------------------

namespace evt
{
	EventGenerator::EventGenerator()
		: events_()
		, timeUntilNextEvent_()
		, timeBeetweenEvents_(BASE_TIME_BEETWEEN_EVENTS)
		, chanceSum_(0)
	{
		events_ = generateEvents();

		timeUntilNextEvent_ = seconds(normalRandFloat(FIRST_EVENT_TIME,
		                                              FIRST_EVENT_TIME_DEV));

		computeChanceSum();
	}

	void EventGenerator::computeChanceSum()
	{
		chanceSum_ = 0;
		for(const auto& evt : events_)
		{
			chanceSum_ += evt.chance;
		}
	}


//-----------------------------------------------------------------------------

	Event EventGenerator::update(Time dt) 
	{
		timeUntilNextEvent_ -= dt;

		if(timeUntilNextEvent_ < Time())
		{
			auto iEvt = chooseEvent();
			updateCounters(iEvt);
			return events_[iEvt];
		}

		Event e;
		e.diff = Event::None;
		return e;
	}

	void EventGenerator::updateDifficulty(DifficultyEventGen diff)
	{
		updateChances(Event::Easy, diff.probaEasy);
		updateChances(Event::Medium, diff.probaMedium);
		updateChances(Event::Hard, diff.probaHard);
		computeChanceSum();

		timeBeetweenEvents_ = diff.delay;
	}

	// I love shiny functional C++14! But g++4.8.2 doesn't support everything (yet).
	// That's why some things are commented.
	void EventGenerator::updateChances(Event::Difficulty diff, unsigned int chance)
	{
		std::for_each(events_.begin(),
		              events_.end(),
//		              [=](auto& event)
		              [=](Event& event)
		              {
			              if(event.diff == diff)
				              event.chance = chance;
		              });
	}


//-----------------------------------------------------------------------------

	void EventGenerator::updateCounters(std::size_t nextEvt)
	{
		float delay = normalRandFloat(timeBeetweenEvents_, timeBeetweenEvents_ / 5);
		delay = std::max(0.f, delay);

		timeUntilNextEvent_ += events_[nextEvt].getDuration() + seconds(delay);
	}


	std::size_t EventGenerator::chooseEvent() const
	{
		int rint = randInt(1, chanceSum_);

		int sum = 0;
		for(std::size_t i=0; i<events_.size(); ++i)
		{
			sum += events_[i].chance;
			if (rint < sum)
				return i;
		}

		return 0;
	}
}

