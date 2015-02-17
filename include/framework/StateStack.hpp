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
	void registerState(StateID id);

	void update(sf::Time dt);
	void draw();
	void handleEvent(const sf::Event& event);
	
	void pushState(StateID id);
	void popState();
	void clearStates();

	bool isEmpty() const;

private:
	State::UPtr createState(StateID id);
	void applyPendingChanges();

private:
	struct PendingChange
	{
		PendingChange(Action action, StateID id = StateID::None);

		Action action;
		StateID id;
	};

private:
	std::vector<State::UPtr> stack_;
	std::vector<PendingChange> pendingList_;
	
	State::Context context_;
	std::map<StateID, std::function<State::UPtr()>> factories_;
};

template<typename T>
void StateStack::registerState(StateID id)
{
	factories_[id] = 
		[this]()
		{
			return State::UPtr(new T(*this, context_));
		};
}

#endif // STATE_STACK_HPP
