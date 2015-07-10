#ifndef GUI_TRANSITION_DEQUE_HPP
#define GUI_TRANSITION_DEQUE_HPP


#include <deque>

#include "gui/Transition.hpp"

namespace gui
{
	class TransitionDeque
	{
	public:
		TransitionDeque(TransformData origin, TransformData space, Transition::Type type, Time duration);

		void update(Time dt);
		
		void pushBack(sf::Transformable* transformable);
		void popFront();

		void setDuration(Time duration);

	private:
		const TransformData ORIGIN_;
		const TransformData SPACE_;
		const Transition::Type TYPE_;

	private:
		Time duration_;
		std::deque<Transition> deque_;
	};
}


#endif // GUI_TRANSITION_DEQUE_HPP
