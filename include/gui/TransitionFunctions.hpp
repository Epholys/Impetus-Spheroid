#ifndef GUI_TRANSITION_FUNCTIONS_HPP
#define GUI_TRANSITION_FUNCTIONS_HPP


#include <functional>
#include <utility>

#include "gui/Transition.hpp"
#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

namespace gui
{
	std::function<Vector2f(Time)>
	generateTransitionFunction(Transition::Type type,
	                           Vector2f start,
	                           Vector2f finish,
	                           Time duration);
}


#endif // GUI_TRANSITION_FUNCTIONS_HPP
