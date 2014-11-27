#include "core/EventGenerator.hpp"


//-----------------------------------------------------------------------------

namespace evt
{
	EventGenerator::EventGenerator()
		: events_()
		, timeUntilNextEvent_()
		, chanceSum_(0)
	{
		events_ = generateEvents();

		for(const auto& evt : events_)
		{
			chanceSum_ += evt.chance;
		}

		timeUntilNextEvent_ = seconds(normalRandFloat(5.f, 2.f));
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

		return Event();
	}

//-----------------------------------------------------------------------------

	void EventGenerator::updateCounters(std::size_t nextEvt)
	{
		float delay = normalRandFloat(5.f, 2.f);
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
