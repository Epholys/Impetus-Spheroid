#include "gui/TransitionFunctions.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		std::function<TransformData(Time)>
		linearTransition(const TransformData& start,
			             const TransformData& finish,
			             Time duration)
		{
			return
				[=](Time)
				{
					return TransformData(linearFactor(start.position, finish.position, duration),
					                     linearFactor(start.angle, finish.angle, duration),
					                     linearFactor(start.scale, finish.scale, duration));
				};
		}
	}

	std::function<TransformData(Time)>
	generateTransitionFunction(Transition::Type type,
	                           const TransformData& start,
	                           const TransformData& finish,
	                           Time duration)
	{
		switch(type)
		{
		case Transition::Linear:
			return std::move(linearTransition(start, finish, duration));
		default:
			return nullptr;
		}
	}
}
   
