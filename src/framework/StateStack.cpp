#include "framework/StateStack.hpp"
#include "framework/Assertion.hpp"


//-----------------------------------------------------------------------------

StateStack::StateStack(State::Context context)
	: stack_()
	, pendingList_()
	, context_(context)
	, factories_()
{
}


//-----------------------------------------------------------------------------


bool StateStack::isEmpty() const
{
	return stack_.empty();
}


//-----------------------------------------------------------------------------


void StateStack::pushState(StateID::ID id)
{
	pendingList_.push_back(PendingChange(Push, id));
}

void StateStack::popState()
{
	pendingList_.push_back(PendingChange(Pop));
}

void StateStack::clearStates()
{
	pendingList_.push_back(PendingChange(Clear));
}

//-----------------------------------------------------------------------------

void StateStack::update(sf::Time dt)
{
	for (auto it=stack_.rbegin(); it != stack_.rend(); ++it)
	{
		if (!(*it)->update(dt))
			break;
	}

	applyPendingChanges();
}

void StateStack::draw(sf::RenderStates states)
{
	for (auto& state : stack_)
		state->draw(states);
}

void StateStack::handleEvent(const sf::Event& event)
{
	for (auto it=stack_.rbegin(); it != stack_.rend(); ++it)
	{
		if (!(*it)->handleInput(event))
			break;
	}

	applyPendingChanges();
}


//-----------------------------------------------------------------------------

State::UPtr StateStack::createState(StateID::ID id)
{
	auto found = factories_.find(id);
	assert(found != factories_.end());

	return found->second();
}

void StateStack::applyPendingChanges()
{
	for (const auto& change : pendingList_)
	{
		switch (change.action)
		{
			case Push:
				stack_.push_back(createState(change.id));
				break;

			case Pop:
				stack_.pop_back();
				break;

			case Clear:
				stack_.clear();
				break;
		}
	}

	pendingList_.clear();
}

StateStack::PendingChange::PendingChange(Action action, StateID::ID id)
: action(action)
, id(id)
{
}
