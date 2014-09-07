#include <SFML/Window/Event.hpp>

#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: window_(sf::VideoMode(800, 600),
	          "Impetus Spheroid",
	          sf::Style::Titlebar | sf::Style::Close,
	          sf::ContextSettings(0, 0, 4))
{
}


//-----------------------------------------------------------------------------
// *** Game loop: ***

void Application::run()
{
	sf::Clock clock;
	sf::Time elapsedTime = sf::Time::Zero;
	while (window_.isOpen())
	{
		handleInput();
		
		elapsedTime = clock.restart();
		update(elapsedTime);
		
		render();
	}
}


//-----------------------------------------------------------------------------
// *** Private functions: ***

void Application::handleInput()
{
	sf::Event event;
	while (window_.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window_.close();
		}
	}
}

void Application::update(sf::Time)
{
	
}

void Application::render()
{
	window_.clear();
	window_.display();
}
