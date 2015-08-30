#ifndef GUI_MULTI_TRANSITION_HPP
#define  GUI_MULTI_TRANSITION_HPP


#include <deque>

#include "gui/Transition.hpp"

namespace gui
{
	class MultiTransition
	{
	public:
		MultiTransition();

		void addTransition(const Transition& transition);

		void update(Time dt);
		
		void setTransformable(sf::Transformable* toMove);
		bool isOver() const;
		Time getDuration() const;

	private:
		std::deque<Transition> transitions_;
		sf::Transformable* toMove_;
	};
} // gui::


#endif // GUI_MULTI_TRANSITION_HPP
