#ifndef STATE_MARKET_HPP
#define STATE_MARKET_HPP

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/State.hpp"
#include "framework/DataSaver.hpp"
#include "core/Market.hpp"

class StateMarket : public State
{
public:
	StateMarket(StateStack& stack, Context context);
	virtual ~StateMarket();

	virtual void draw();
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);

private:
	Market market_;
};


#endif // STATE_MARKET_HPP
