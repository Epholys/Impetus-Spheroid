#include "framework/StateGame.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(*context.window)
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
	return true;
}

bool StateGame::handleInput(const sf::Event& event)
{
	world_.handleInput(event);
	return true;
}
