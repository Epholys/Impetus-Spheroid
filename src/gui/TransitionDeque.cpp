#include "gui/TransitionDeque.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	TransitionDeque::TransitionDeque(TransformData origin, TransformData space, Transition::Type type, Time duration)
		: ORIGIN_(origin)
		, SPACE_(space)
		, TYPE_(type)
		, duration_(duration)
		, deque_()
	{
	}


//-----------------------------------------------------------------------------

	void TransitionDeque::setDuration(Time duration)
	{
		duration_ = duration;
	}


//-----------------------------------------------------------------------------
	
	void TransitionDeque::update(Time dt)
	{
		for(auto& transition : deque_)
		{
			transition.update(dt);
		}
	}


//-----------------------------------------------------------------------------

	void TransitionDeque::pushBack(sf::Transformable* transformable)
	{
		if(deque_.empty())
		{
			deque_.push_back(Transition(transformable,
			                            TYPE_,
			                            ORIGIN_,
			                            ORIGIN_,
			                            Time()));
		}
		else
		{
			const Transition& lastTransition = deque_.back();
			deque_.push_back(Transition(transformable,
			                            TYPE_,
			                            lastTransition.getStart() + SPACE_,
			                            lastTransition.getFinish() + SPACE_,
			                            lastTransition.getDuration()));
			deque_.back().update(lastTransition.getAccumulatedTime());
		}
	}

	void TransitionDeque::popFront()
	{
		if(deque_.empty())
			return;
		
		deque_.pop_front();
		for(std::size_t i=0; i<deque_.size(); ++i)
		{
			deque_[i].updateFinish(ORIGIN_ + SPACE_ * static_cast<float>(i), duration_);
		}
	}
}
