#include "gui/Transition.hpp"
#include "gui/TransitionFunctions.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	Transition::Transition(sf::Transformable* toMove,
	           Type type,
	           Vector2f start,
	           Vector2f finish,
	           Time duration)
		: toMove_(toMove)
		, type_(type)
		, start_(start)
		, finish_(finish)
		, duration_(duration)
		, accumulatedTime_()
		, transitionFunction_(generateTransitionFunction(type, start, finish, duration))
	{
	}

	void Transition::update(Time dt)
	{
		if(accumulatedTime_ >= duration_ || !transitionFunction_ || !toMove_)
			return;

		accumulatedTime_ += dt;

		if(accumulatedTime_ >= duration_)
		{
			toMove_->setPosition(finish_);
		}
		else
		{
			toMove_->move(transitionFunction_(accumulatedTime_) * dt.asSeconds());
		}
	}

	void Transition::updateFinish(Vector2f newFinish, Time toAdd)
	{
		finish_ = newFinish;
		duration_ += toAdd;
		transitionFunction_ = generateTransitionFunction(type_, start_, finish_, duration_);
	}

	bool Transition::isOver() const
	{
		return (accumulatedTime_ >= duration_);
	}
}
