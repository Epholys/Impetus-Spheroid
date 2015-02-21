#include "framework/StateOver.hpp"


//-----------------------------------------------------------------------------

StateOver::StateOver(StateStack& stack, Context context)
	: State(stack, context)
	, font_()
	, gameOver_()
	, retry_()
{
	Vector2u winSize = Vector2u(context.window->getSize());
	const Vector2f GAME_OVER_POS (winSize.x / 2, winSize.y / 2);
	const Vector2f RETRY_POS (winSize.x / 2, 2 * winSize.y / 3); 

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	gameOver_.setFont(font_);
	gameOver_.setString("G A M E  O V E R");
	gameOver_.setColor(sf::Color::White);
	centerOrigin(gameOver_);
	gameOver_.setPosition(GAME_OVER_POS);

	retry_.setFont(font_);
	retry_.setString("Press SPACE to retry");
	retry_.setColor(sf::Color::White);
	centerOrigin(retry_);
	retry_.setPosition(RETRY_POS);
}

StateOver::~StateOver()
{
}


//-----------------------------------------------------------------------------

void StateOver::draw()
{
	sf::RectangleShape rect (Vector2f(context_.window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	context_.window->draw(rect);
	context_.window->draw(gameOver_);
	context_.window->draw(retry_);
}

bool StateOver::update(Time)
{
	return true;
}

bool StateOver::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased &&
	   event.key.code == sf::Keyboard::Space)
	{
		requestStackPop();
		requestStackPop();
		requestStackPush(StateID::Game);
	}
	return false;
}

