#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP


#include <vector>
#include <unordered_map>
#include <functional>

#include <SFML/Graphics/RenderStates.hpp>

#include "framework/State.hpp"


/* Simple Stack for the different State of the game.
 * 
 * The Stack architecture allows several states to be active at the same
 * time. Moreover, the uppermost State can decide whether the state below it can
 * update or handling an event itself, through the boolean returned by
 * State::update() and State::handleInput().
 *
 * All operations on stack_ via push/pop/clear are done in the update() method
 * not to have some problems with iteration, as the State can do this
 *
 * The States must be registered before being able to create them via
 * pushState(), as they are not hard-coded into the program.
 * 
 * */

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
	void draw(sf::RenderStates states);
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
	std::unordered_map<StateID::ID, std::function<State::UPtr()>, std::hash<unsigned int>> factories_;
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
