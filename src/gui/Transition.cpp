#include "gui/Transition.hpp"
#include "gui/TransitionFunctions.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	TransformData operator+(const TransformData& left, const TransformData& right)
	{
		return TransformData(left.position + right.position,
		                     left.angle + right.angle,
		                     left.scale + right.scale);
	}
	TransformData operator*(const TransformData& left, float right)
	{
		return TransformData(left.position * right,
		                     left.angle * right,
		                     left.scale * right);
	}

	//-----------------------------------------------------------------------------
	
	Transition::Transition(sf::Transformable* toMove,
	                       Type type,
	                       const TransformData& start,
	                       const TransformData& finish,
	                       Time duration)
		: toMove_(toMove)
		, type_(type)
		, start_(start)
		, finish_(finish)
		, duration_(duration)
		, accumulatedTime_()
		, transitionFunction_(generateTransitionFunction(type_, start_, finish_, duration_))
	{
		if(toMove)
		{
			setTransformation(start);
		}
	}

	void Transition::update(Time dt)
	{
		if(accumulatedTime_ >= duration_ || !transitionFunction_ || !toMove_)
			return;

		accumulatedTime_ += dt;

		if(accumulatedTime_ >= duration_)
		{
			setTransformation(finish_);
		}
		else
		{
			applyTransformation(transitionFunction_(accumulatedTime_), dt);
		}
	}

	void Transition::updateFinish(const TransformData& newFinish, Time toAdd)
	{
		finish_ = newFinish;
		duration_ += toAdd;
		transitionFunction_ = generateTransitionFunction(type_, start_, finish_, duration_);
	}

	bool Transition::isOver() const
	{
		return accumulatedTime_ >= duration_;
	}
	
	TransformData Transition::getFinish() const
	{
		return finish_;
	}

	TransformData Transition::getStart() const
	{
		return start_;
	}
	
	Time Transition::getAccumulatedTime() const
	{
		return accumulatedTime_;
	}

	Time Transition::getDuration() const
	{
		return duration_;
	}

	void Transition::setTransformation(const TransformData& data)
	{
		if(!toMove_)
			return;
		
		toMove_->setPosition(data.position);
		toMove_->setRotation(data.angle);
		toMove_->setScale(data.scale);
	}

	void Transition::applyTransformation(const TransformData& data, Time dt)
	{
		if(!toMove_)
			return;

		float seconds = dt.asSeconds();
		toMove_->move(data.position * seconds);
		toMove_->rotate(data.angle * seconds);
		toMove_->setScale(start_.scale + data.scale * seconds);
	}	  
}

