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

	Transition::Transition()
		: Transition(nullptr, None, TransformData(), TransformData(), Time())
	{}
	
	Transition::Transition(sf::Transformable* toMove,
	                       Type type,
	                       TransformData start,
	                       TransformData finish,
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
			applyTransformation(transitionFunction_(accumulatedTime_, dt));
		}
	}

	void Transition::updateFinish(const TransformData& newFinish, Time toAdd)
	{
		if(!toMove_)
			return;
		
		finish_ = newFinish;
		start_ = TransformData(Vector2f(toMove_->getPosition()),
		                       toMove_->getRotation(),
		                       Vector2f(toMove_->getScale()));
		duration_ = duration_ - accumulatedTime_ + toAdd;
		accumulatedTime_ = Time();
		transitionFunction_ = generateTransitionFunction(type_, start_, finish_, duration_);
	}

	void Transition::setTransformable(sf::Transformable* toMove)
	{
		toMove_ = toMove;
		setTransformation(start_);
		accumulatedTime_ = Time();
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

	void Transition::applyTransformation(const TransformData& data)
	{
		if(!toMove_)
			return;
		toMove_->move(data.position);
		toMove_->rotate(data.angle);
		toMove_->setScale(toMove_->getScale() + data.scale);
	}	  
}

