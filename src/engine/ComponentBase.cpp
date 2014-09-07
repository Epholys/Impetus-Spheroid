#include "engine/ComponentBase.hpp"


namespace eg
{

	ComponentBase::ComponentBase()
		: pauseCounter(Time::Zero)
	{
	}

//-----------------------------------------------------------------------------
// *** The pause system: ***

	void ComponentBase::update(Time dt)
	{
		if(isPaused())
		{
			pauseCounter -= dt;
		}
	}

	void ComponentBase::pause(Time t)
	{
		pauseCounter = t;
	}

	bool ComponentBase::isPaused() const
	{
		return pauseCounter > Time::Zero;
	}

} // namespace eg
