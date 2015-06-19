#ifndef STATE_GAME_HPP
#define STATE_GAME_HPP


#include "framework/State.hpp"
#include "core/World.hpp"


// The main State, in which the game's World live.

class StateGame : public State
{
public:
	StateGame(StateStack& stack, Context context);
	virtual ~StateGame();

	virtual void draw(sf::RenderStates states);
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:
	World world_;
	bool overScreenUp_;
};


#endif // STATE_GAME_HPP
