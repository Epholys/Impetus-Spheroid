#include "gui/MultiTransition.hpp"

//-----------------------------------------------------------------------------

namespace gui
{
	MultiTransition::MultiTransition()
		: transitions_()
		, toMove_(nullptr)
	{
	}


//-----------------------------------------------------------------------------

	void MultiTransition::addTransition(const Transition& transition)
	{
		transitions_.push_back(transition);
		
		if(transitions_.size() == 1)
		{
			if(!toMove_)
			{
				toMove_ = transitions_.back().getTransformable();
			}
			else
			{
				transitions_.back().setTransformable(toMove_);
			}
		}
	}


//-----------------------------------------------------------------------------

	void MultiTransition::update(Time dt)
	{
		if(transitions_.size() == 0)
			return;
		
		transitions_.front().update(dt);

		if(transitions_.front().isOver())
		{
			transitions_.pop_front();
			if(transitions_.size() != 0)
				transitions_.front().setTransformable(toMove_);
		}
	}


//-----------------------------------------------------------------------------

	void MultiTransition::setTransformable(sf::Transformable* toMove)
	{
		toMove_ = toMove;
		
		if(transitions_.size() == 0)
			return;
		
		transitions_.front().setTransformable(toMove);
	}

	bool MultiTransition::isOver() const
	{
		return transitions_.size() == 0;
	}

	Time MultiTransition::getDuration() const
	{
		Time duration;
		for(auto& transition : transitions_)
		{
			duration += transition.getDuration();
		}
		return duration;
	}
		
} // gui::
