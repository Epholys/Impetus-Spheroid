#include "framework/StateMarket.hpp"


//-----------------------------------------------------------------------------

StateMarket::StateMarket(StateStack& stack, Context context)
	: State(stack, context)
	, market_(context)
{
}

StateMarket::~StateMarket()
{
}


//-----------------------------------------------------------------------------

void StateMarket::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	window->draw(rect);

	window->draw(market_, states);
}

bool StateMarket::update(Time)
{
	return true;
}

bool StateMarket::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased)
	{
		switch(event.key.code)		
		{
		case sf::Keyboard::Space:
		case sf::Keyboard::Escape:
			requestStackClear();
			requestStackPush(StateID::Game);
			DataSaver::saveDatas(*context_.metaData);
			break;

		default:
			break;
		}
	}
	
	market_.handleEvent(event);

	return false;
}
