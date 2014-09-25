#include <SFML/Window/Event.hpp>

#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: window_(sf::VideoMode(800, 600),
	          "Impetus Spheroid",
	          sf::Style::Titlebar | sf::Style::Close,
	          sf::ContextSettings(0, 0, 4))
	, ecs_()
	, engine_(ecs_)
	, balls_()
{
	window_.setKeyRepeatEnabled(false);
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
		// else if (event.type == sf::Event::KeyPressed)
		// {

		// }
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Left)			
		{
			balls_.push_back(std::unique_ptr<Ball>
			                 (new Ball(ecs_,
			                           Vector2f(event.mouseButton.x,
			                                    event.mouseButton.y),
			                           10.f)));
		}
	}
}

void Application::update(sf::Time dt)
{
	engine_.update(dt);
	for (auto& ball : balls_)
	{
		ball->update(dt);
	}	
}

void Application::render()
{
	window_.clear();
	for (const auto& ball : balls_)
	{
		window_.draw(*ball);
	}
	window_.display();
}
