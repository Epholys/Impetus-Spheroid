#include "gui/TransitionFunctions.hpp"
#include <cmath>

//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		std::function<TransformData(Time, Time)>
		linearTransition(const TransformData& start,
			             const TransformData& finish,
			             Time duration)
		{
			return
				[=](Time, Time dt)
				{
					float time = dt.asSeconds();
					return TransformData(linearFactor(start.position, finish.position, duration) * time,
					                     linearFactor(start.angle, finish.angle, duration) * time,
					                     linearFactor(start.scale, finish.scale, duration) * time);
				};
		}

		std::function<TransformData(Time, Time)>
		polynomialTransition(const TransformData& start,
		                     const TransformData& finish,
		                     Time duration,
		                     int power)
		{
			assert(duration != milliseconds(0));
			float varFactor = pow(1/duration.asSeconds(), power);
			Vector2f posDiff = finish.position - start.position;
			float angleDiff = finish.angle - start.angle;
			Vector2f scaleDiff = finish.scale - start.scale;
			
			return
				[=](Time accumulatedTime, Time dt)
				{
					float dtime = dt.asSeconds();
					float acctime = accumulatedTime.asSeconds();
					float acctimePowered = pow(acctime, power-1);
					
					Vector2f dPos = power * varFactor * posDiff * acctimePowered     * dtime;
					float dAngle = power *varFactor * angleDiff * acctimePowered     * dtime;
					Vector2f dScale = power * varFactor * scaleDiff * acctimePowered * dtime;

					return TransformData(dPos, dAngle, dScale);
				};
		}
	}

	std::function<TransformData(Time, Time)>
	generateTransitionFunction(Transition::Type type,
	                           const TransformData& start,
	                           const TransformData& finish,
	                           Time duration)
	{
		switch(type)
		{
		case Transition::Linear:
			return std::move(linearTransition(start, finish, duration));
		case Transition::Quadratic:
			return std::move(polynomialTransition(start, finish, duration, 2));
		default:
			return nullptr;
		}
	}
}
   
