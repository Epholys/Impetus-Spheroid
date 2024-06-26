#include "framework/StatePause.hpp"


//-----------------------------------------------------------------------------

StatePause::StatePause(StateStack& stack, Context context)
	: State(stack, context)
	, pause_()
{
	Vector2u winSize = Vector2u(context_.originalWindowSize);
	const Vector2f PAUSE_POS (winSize.x / 2, winSize.y / 2);

	pause_.setFont(context.fonts->get(FontID::ForcedSquare));
	pause_.setString("Pause");
	centerOrigin(pause_);
	pause_.setPosition(PAUSE_POS);
	pause_.setColor(sf::Color::White);
}

StatePause::~StatePause()
{
}


//-----------------------------------------------------------------------------

void StatePause::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	window->draw(rect);

	window->draw(pause_, states);
}

bool StatePause::update(Time)
{
	return false;
}

bool StatePause::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased ||
	   event.type == sf::Event::MouseButtonReleased)
	{
		requestStackPop();
	}

	return false;
}

