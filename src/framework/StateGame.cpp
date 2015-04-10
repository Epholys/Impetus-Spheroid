#include "framework/StateGame.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(*context.window, *context.inventory)
	, overScreenUp_(false)
{
	context_.inventory->addWorld(&world_);
}

StateGame::~StateGame()
{
}


//------------------------------------------------------------------

void StateGame::draw()
{
	world_.draw();
	context_.window->draw(*context_.inventory);
}

bool StateGame::update(Time dt)
{	
	world_.update(dt);
	if(world_.isGameOver() && !overScreenUp_)
	{
		requestStackPush(StateID::GameOver);
		context_.inventory->removeWorld();
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
	context_.inventory->handleInput(event);
	world_.handleInput(event);
	return true;
}
