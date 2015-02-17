#include "framework/State.hpp"
#include "framework/StateStack.hpp"


//-----------------------------------------------------------------------------

State::Context::Context(sf::RenderWindow& window)
	: window(&window)
{
}


//-----------------------------------------------------------------------------

State::State(StateStack& stack, Context context)
	: stack_(&stack)
	, context_(context)
{
}

State::~State()
{
}


//-----------------------------------------------------------------------------

void State::requestStackPush(StateID::ID id)
{
	stack_->pushState(id);
}

void State::requestStackPop()
{
	stack_->popState();
}

void State::requestStackClear()
{
	stack_->clearStates();
}
