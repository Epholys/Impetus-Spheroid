#ifndef STATE_OVER_HPP
#define STATE_OVER_HPP


#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/State.hpp"
#include "utility/utility.hpp"
#include "utility/Vector2.hpp"


class StateOver : public State
{
public:
	StateOver(StateStack& stack, Context context);
	virtual ~StateOver();

	virtual void draw();
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:
	sf::Font font_;
	sf::Text gameOver_;
	sf::Text retry_;
};


#endif // STATE_OVER_HPP
