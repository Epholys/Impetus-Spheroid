#include "framework/StateGame.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(context.originalWindowSize, *context.datas)
	, overScreenUp_(false)
{
	context_.datas->inventory.addWorld(&world_);
}

StateGame::~StateGame()
{
}


//------------------------------------------------------------------

void StateGame::draw(sf::RenderStates states)
{
	context_.window->draw(world_, states);
	context_.window->draw(context_.datas->inventory, states);
}

bool StateGame::update(Time dt)
{	
	world_.update(dt);
	if(world_.isGameOver() && !overScreenUp_)
	{
		requestStackPush(StateID::GameOver);
		context_.datas->inventory.removeWorld();
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
	context_.datas->inventory.handleInput(event);
	world_.handleInput(event);
	return true;
}
