#ifndef TRANSITIONNABLE_HPP
#define TRANSITIONNABLE_HPP


#include "gui/MultiTransition.hpp"


// Interface to inherit from to ease up transitions

class Transitionnable
{
public:
	Transitionnable(sf::Transformable* child);
	virtual ~Transitionnable();

	void addTransition(const gui::Transition& transition);

protected:
	void update(Time dt);
	
protected:
	gui::MultiTransition transitions_;
};


#endif // TRANSITIONNABLE_HPP
