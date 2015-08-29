#include "framework/StateGame.hpp"
#include "framework/StateTutorial.hpp"
#include "framework/StateStack.hpp"


//-----------------------------------------------------------------------------

StateGame::StateGame(StateStack& stack, Context context)
	: State(stack, context)
	, world_(context.originalWindowSize, *context.metaData, *context.fonts, *context.textures)
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
	if(typeid(*stack_->upperState()) == typeid(StateTutorial))
		world_.setState(World::WaitingTutorial);
	
	world_.update(dt);
	if(world_.isGameOver() && !overScreenUp_)
	{
		requestStackPush(StateID::GameOver);
		*context_.lastGameData = world_.getGameStats();
		context_.metaData->inventory.removeWorld();
		overScreenUp_ = true;
	}
	return true;
}

bool StateGame::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::MouseLeft)
	{
		requestStackPush(StateID::Pause);
		return true;
	}
	context_.metaData->inventory.handleInput(event);
	world_.handleInput(event);
	return true;
}
