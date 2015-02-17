#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP


#include <vector>
#include <map>

#include "framework/State.hpp"


class StateStack
{
public:
	enum Action
	{
		Push,
		Pop,
		Clear,
	};

public:
	StateStack(State::Context context);

	template<typename T>
	void registerState(StateID::ID id);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);
	
	void pushState(StateID::ID id);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::UPtr createState(StateID::ID id);
	void applyPendingChanges();

private:
	struct PendingChange
	{
		PendingChange(Action action, StateID::ID id = StateID::None);

		Action action;
		StateID::ID id;
	};

private:
	std::vector<State::UPtr> stack_;
	std::vector<PendingChange> pendingList_;
	
	State::Context context_;
	std::map<StateID::ID, std::function<State::UPtr()>> factories_;
};

template<typename T>
void StateStack::registerState(StateID::ID id)
{
	factories_[id] = 
		[this]()
		{
			return State::UPtr(new T(*this, context_));
		};
}

#endif // STATE_STACK_HPP
