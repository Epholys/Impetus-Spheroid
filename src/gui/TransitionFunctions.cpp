#include "gui/TransitionFunctions.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		std::function<Vector2f(Time)>
		linearTransition(Vector2f start, Vector2f finish, Time duration)
		{
			return
				[=](Time)
				{
					return (finish - start) / duration.asSeconds();
				};
		}
	}

	std::function<Vector2f(Time)>
	generateTransitionFunction(Transition::Type type,
	                           Vector2f start,
	                           Vector2f finish,
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
   
