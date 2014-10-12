#include <iostream>

#include <SFML/Window/Event.hpp>

#include "framework/Assertion.hpp"
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
	, ballMass_(1.f)
	, ballRadius_(10.f)
	, ballColor_(sf::Color::Red)
	, deltaMouseLine_()
	, rects_()
{
	window_.setKeyRepeatEnabled(false);
	window_.setVerticalSyncEnabled(false);

	deltaMouseLine_.setPointCount(4);
	deltaMouseLine_.setPoint(0, sf::Vector2f(0,0));
	deltaMouseLine_.setPoint(1, sf::Vector2f(0,0));
	deltaMouseLine_.setPoint(2, sf::Vector2f(0,0));
	deltaMouseLine_.setPoint(3, sf::Vector2f(0,0));
	deltaMouseLine_.setOutlineThickness(1);
	deltaMouseLine_.setOutlineColor(sf::Color::Transparent);
	deltaMouseLine_.setFillColor(sf::Color::Transparent);
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
				ecs_.pauseAllComponents(ecs::Component::Velocity, sf::seconds(2));
				break;
				
			case sf::Keyboard::Add:
				ballMass_ *= 2.f;
				ballRadius_ *= 1.5f;
				break;

			case sf::Keyboard::Subtract:
				ballMass_ /= 2.f;
				ballRadius_ /= 1.5f;
				break;

			case sf::Keyboard::G:
				ballColor_ = (ballColor_ == sf::Color::Green) ? sf::Color::Red : sf::Color::Green;
				break;
				
			case sf::Keyboard::R:
				balls_.clear();
				rects_.clear();
				break;

			default:
				break;
			}
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Left)			
		{
			deltaMouse_ = Vector2f(event.mouseButton.x, event.mouseButton.y);

			deltaMouseLine_.setPoint(0, sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
			deltaMouseLine_.setPoint(1, sf::Vector2f(event.mouseButton.x+1, event.mouseButton.y));
			deltaMouseLine_.setPoint(2, sf::Vector2f(event.mouseButton.x+1, event.mouseButton.y+1));
			deltaMouseLine_.setPoint(3, sf::Vector2f(event.mouseButton.x+1, event.mouseButton.y+1));
			deltaMouseLine_.setOutlineColor(sf::Color::White);
		}
		else if (event.type == sf::Event::MouseButtonReleased &&
		         event.mouseButton.button == sf::Mouse::Left)
		{
			std::unique_ptr<Ball> pBall	(new Ball(ecs_, deltaMouse_, ballRadius_, ballMass_, ballColor_));
			deltaMouse_ -= Vector2f(event.mouseButton.x, event.mouseButton.y);
			auto entBall = pBall->getLabel();
			auto velComp = dynCast<ecs::Velocity>
				(ecs_.getComponent(entBall, ecs::Component::Velocity));
			assert(velComp);
			velComp->velocity_ -= 3.f * deltaMouse_ / ballMass_;
			balls_.push_back(std::move(pBall));

			deltaMouseLine_.setOutlineColor(sf::Color::Transparent);
			deltaMouseLine_.setFillColor(sf::Color::Transparent);
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Right)			
		{
			deltaMouse_ = Vector2f(event.mouseButton.x, event.mouseButton.y);
			std::unique_ptr<Rectangle> pRect(new Rectangle(ecs_, deltaMouse_, Vector2f(15.f, 75.f), sf::Color::Blue));
			rects_.push_back(std::move(pRect));
		}
		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Middle)			
		{
			deltaMouse_ = Vector2f(event.mouseButton.x, event.mouseButton.y);
			std::unique_ptr<Rectangle> pRect(new Rectangle(ecs_, deltaMouse_, Vector2f(800.f, 15.f), sf::Color::Blue));
			rects_.push_back(std::move(pRect));
		}

	}


	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window_));
		deltaMouseLine_.setPoint(2, sf::Vector2f(mousePos.x, mousePos.y));
		deltaMouseLine_.setPoint(3, sf::Vector2f(mousePos.x+1, mousePos.y+1));
	}
}

void Application::update(sf::Time dt)
{
	ecs_.update(dt);
	engine_.update(dt);

	auto it = balls_.begin();
	while(it != balls_.end())
	{
		auto ballPos = ecs_.getComponent((*(it))->getLabel(), ecs::Component::Position);
		if(ballPos)
		{
			auto ballPosComp = dynCast<ecs::Position>(ballPos);
			assert(ballPosComp);
			
			sf::Vector2f windowSize(window_.getSize());
			float ballXPos = ballPosComp->position_.x;
			float ballYPos = ballPosComp->position_.y;
			if(ballXPos < -ballRadius_ ||
			   ballXPos > windowSize.x + ballRadius_ ||
			   ballYPos > windowSize.y + ballRadius_)
			{
				balls_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

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
	for (const auto& rect : rects_)
	{
		window_.draw(*rect);
	}
	window_.draw(deltaMouseLine_);
	window_.display();
}
