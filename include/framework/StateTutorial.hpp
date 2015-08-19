#ifndef STATE_TUTORIAL_HPP
#define STATE_TUTORIAL_HPP


#include "framework/State.hpp"


// The Tutorial State, which simply iterate and draw from the TutorialDatas

class StateTutorial : public State
{
public:
	StateTutorial(StateStack& stack, Context context);
	virtual ~StateTutorial();

	virtual void draw(sf::RenderStates states);
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:

};


#endif // STATE_TUTORIAL_HPP
