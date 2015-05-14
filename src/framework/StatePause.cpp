#include "framework/StatePause.hpp"


//-----------------------------------------------------------------------------

StatePause::StatePause(StateStack& stack, Context context)
	: State(stack, context)
	, font_()
	, pause_()
{
	Vector2u winSize = Vector2u(context_.originalWindowSize);
	const Vector2f PAUSE_POS (winSize.x / 2, winSize.y / 2);

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	pause_.setFont(font_);
	pause_.setString("Pause");
	pause_.setColor(sf::Color::White);
	centerOrigin(pause_);
	pause_.setPosition(PAUSE_POS);
}

StatePause::~StatePause()
{
}


//-----------------------------------------------------------------------------

void StatePause::draw()
{
	sf::RenderStates states;
	states.transform *= *context_.globalTransform;
	sf::RectangleShape rect (Vector2f(context_.window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	context_.window->draw(rect);
	context_.window->draw(pause_, states);
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

