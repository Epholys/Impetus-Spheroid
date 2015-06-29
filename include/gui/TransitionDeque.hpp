#ifndef GUI_TRANSITION_DEQUE_HPP
#define GUI_TRANSITION_DEQUE_HPP


#include <deque>

#include "gui/Transition.hpp"

namespace gui
{
	class TransitionDeque
	{
	public:
		TransitionDeque(Vector2f origin, Vector2f space, Transition::Type type, Time duration);

		void update(Time dt);
		
		void pushBack(sf::Transformable* transformable);
		void popFront();

	private:
		const Vector2f ORIGIN_;
		const Vector2f SPACE_;
		const Transition::Type TYPE_;
		const Time DURATION_;

	private:
		std::deque<Transition> deque_;
	};
}


#endif // GUI_TRANSITION_DEQUE_HPP
