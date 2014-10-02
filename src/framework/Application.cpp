
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
	, deltaMouse_()
	, balls_()
{
	window_.setKeyRepeatEnabled(false);
	window_.setVerticalSyncEnabled(false);
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
		else if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::P:
				ecs_.pauseAllComponents(sf::seconds(2));
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Left)			
		{
			deltaMouse_ = Vector2f(event.mouseButton.x, event.mouseButton.y);
		}
		else if (event.type == sf::Event::MouseButtonReleased &&
		         event.mouseButton.button == sf::Mouse::Left)
		{
			std::unique_ptr<Ball> pBall	(new Ball(ecs_, deltaMouse_, 10.f));
			deltaMouse_ -= Vector2f(event.mouseButton.x, event.mouseButton.y);
			auto entBall = pBall->getLabel();
			auto velComp = dynCast<ecs::Velocity>
				(ecs_.getComponent(entBall, ecs::Component::Velocity));
			velComp->velocity_ -= 3.f * deltaMouse_;			
			balls_.push_back(std::move(pBall));
		}
	}
}

void Application::update(sf::Time dt)
{
	ecs_.update(dt);
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
