#ifndef GUI_TRANSITION_FUNCTIONS_HPP
#define GUI_TRANSITION_FUNCTIONS_HPP


#include <functional>
#include <utility>

#include "gui/Transition.hpp"
#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

namespace gui
{
	template<typename T>
	T linearFactor(const T& start, const T& finish, Time duration)
	{
		return (finish - start) / duration.asSeconds();
	}

	std::function<TransformData(Time)>
	generateTransitionFunction(Transition::Type type,
	                           const TransformData& start,
	                           const TransformData& finish,
	                           Time duration);
}


#endif // GUI_TRANSITION_FUNCTIONS_HPP
