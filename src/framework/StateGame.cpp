#include "framework/StateGame.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(*context.window)
	, overScreenUp_(false)
{
}

StateGame::~StateGame()
{
}


//------------------------------------------------------------------

void StateGame::draw()
{
	world_.draw();
}

bool StateGame::update(Time dt)
{	
	world_.update(dt);
	if(world_.isGameOver() && !overScreenUp_)
	{
		requestStackPush(StateID::GameOver);
		overScreenUp_ = true;
	}
	return true;
}

bool StateGame::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::LostFocus)
	{
		requestStackPush(StateID::Pause);
	}
	world_.handleInput(event);
	return true;
}
