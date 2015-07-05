#ifndef STATE_PAUSE_HPP
#define STATE_PAUSE_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/State.hpp"
#include "utility/utility.hpp"
#include "utility/Vector2.hpp"


// A very simple State to pause the game.

class StatePause : public State
{
public:
	StatePause(StateStack& stack, Context context);
	virtual ~StatePause();

	virtual void draw(sf::RenderStates states);
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:
	sf::Text pause_;
};


#endif // STATE_PAUSE_HPP
