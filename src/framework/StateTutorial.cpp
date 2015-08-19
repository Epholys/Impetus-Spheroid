#include "framework/StateTutorial.hpp"


//-----------------------------------------------------------------------------

namespace
{

}


//-----------------------------------------------------------------------------

StateTutorial::StateTutorial(StateStack& stack, Context context)
	: State(stack, context)
{
}

StateTutorial::~StateTutorial()
{
}


//-----------------------------------------------------------------------------

void StateTutorial::draw(sf::RenderStates)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	window->draw(rect);
}

bool StateTutorial::update(Time)
{
	return true;
}

bool StateTutorial::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::MouseButtonReleased &&
	   event.mouseButton.button == sf::Mouse::Left)
	{
		requestStackPop();
	}

	return false;
}

