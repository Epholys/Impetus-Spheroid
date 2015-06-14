#include "framework/StateGame.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(context.originalWindowSize, *context.metaData)
	, overScreenUp_(false)
{
	context_.metaData->inventory.addWorld(&world_);
}

StateGame::~StateGame()
{
}


//------------------------------------------------------------------

void StateGame::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;
	window->draw(world_, states);
	window->draw(context_.metaData->inventory, states);
}

bool StateGame::update(Time dt)
{	
	world_.update(dt);
	if(world_.isGameOver() && !overScreenUp_)
	{
		requestStackPush(StateID::GameOver);
		context_.metaData->inventory.removeWorld();
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
	context_.metaData->inventory.handleInput(event);
	world_.handleInput(event);
	return true;
}
