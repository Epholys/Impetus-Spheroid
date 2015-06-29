#include "gui/TransitionDeque.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	TransitionDeque::TransitionDeque(Vector2f origin, Vector2f space, Transition::Type type, Time duration)
		: ORIGIN_(origin)
		, SPACE_(space)
		, TYPE_(type)
		, DURATION_(duration)
		, deque_()
	{
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
		// if(!deque_.empty())
		// {
		// 	const Transition& lastTransition = deque_.back();
		// 	deque_.push_back(Transition(transformable,
		// 	                            TYPE_,
		// 	                            lastTransition.getStart() + SPACE_,
		// 	                            lastTransition.getFinish() + SPACE_,
		// 	                            lastTransition.getDuration() + DURATION_));
		// 	deque_.back().update(std::min(DURATION_, lastTransition.getAccumulatedTime()));
		// }
		if(deque_.empty())
		{
			deque_.push_back(Transition(transformable,
			                            TYPE_,
			                            ORIGIN_,
			                            ORIGIN_,
			                            Time()));
		}
		// else if(deque_.back().isOver())
		// {
		// 	Vector2f position = ORIGIN_ + SPACE_ * static_cast<float>(deque_.size());
		// 	deque_.push_back(Transition(transformable,
		// 	                            TYPE_,
		// 	                            position,
		// 	                            position,
		// 	                            Time()));	
		// }
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
			deque_[i].updateFinish(ORIGIN_ + static_cast<float>(i) * SPACE_, DURATION_);
		}
	}
}
