#ifndef STATE_TUTORIAL_HPP
#define STATE_TUTORIAL_HPP


#include "data/TutorialData.hpp"
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
	void updateInstruction();
	
private:
	std::size_t index_;
	sf::Text instruction_;
	sf::Sprite arrow_;
};


#endif // STATE_TUTORIAL_HPP
