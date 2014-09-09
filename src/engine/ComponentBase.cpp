#include "engine/ComponentBase.hpp"


namespace eg
{

	ComponentBase::ComponentBase()
		: pauseCounter_(Time::Zero)
	{
	}

	ComponentBase::~ComponentBase()
	{
	}

//-----------------------------------------------------------------------------
// *** The pause system: ***

	void ComponentBase::update(Time dt)
	{
		if(isPaused())
		{
			pauseCounter_ -= dt;
			if (pauseCounter_ < Time::Zero)
			{
				pauseCounter_ = Time::Zero;
			}
		}
	}

	void ComponentBase::pause(Time t)
	{
		pauseCounter_ += t;
	}

	bool ComponentBase::isPaused() const
	{
		return pauseCounter_ > Time::Zero;
	}

} // namespace eg
