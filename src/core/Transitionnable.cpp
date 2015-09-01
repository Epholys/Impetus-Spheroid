#include "core/Transitionnable.hpp"


//-----------------------------------------------------------------------------

Transitionnable::Transitionnable(sf::Transformable* child)
	: transitions_()
{
	transitions_.setTransformable(child);
}

Transitionnable::~Transitionnable()
{
}


//-----------------------------------------------------------------------------

void Transitionnable::addTransition(const gui::Transition& transition)
{
	transitions_.addTransition(transition);
}

void Transitionnable::update(Time dt)
{
	transitions_.update(dt);
}
