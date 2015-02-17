#include "framework/Assertion.hpp"
#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: window_(sf::VideoMode(800, 600),
	          "Impetus Spheroid",
	          sf::Style::Titlebar | sf::Style::Close,
	          sf::ContextSettings(0, 0, 4))
	, stack_()
	, gameWorld_(window_)
{

}


//-----------------------------------------------------------------------------
// *** Game loop: ***

void Application::run()
{
	sf::Clock clock;
	while (window_.isOpen())
	{
		handleInput();

		sf::Time elapsedTime = clock.restart();
		update(elapsedTime);

		render();
	}
}


//-----------------------------------------------------------------------------
// *** Private functions: ***

void Application::handleInput()
{
	gameWorld_.handleInput();
}

void Application::update(sf::Time dt)
{
	gameWorld_.update(dt);
}

void Application::render()
{
	window_.clear(sf::Color(230,230,230));

	gameWorld_.draw();

	window_.display();
}
