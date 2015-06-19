#include "core/Event.hpp"

namespace evt
{
	Event::Event()
		: worldModifiers()
		, chance (0)
	{
	}

	Time Event::getDuration() const
	{
		Time maxDuration;
		for(const auto& worldMod : worldModifiers)
		{
			maxDuration = std::max(maxDuration, worldMod.preDelay_ + worldMod.duration_);
		}
		return maxDuration;
	}
}
