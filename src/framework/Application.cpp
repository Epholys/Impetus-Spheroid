#include <sstream>
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
	, gameWorld_(ecs_, window_)
	, font_()
	, score_(0)
	, scoreText_()
	, balls_()
	, ballMass_(1.f)
	, ballRadius_(10.f)
	, ballColor_(sf::Color::Red)
	, rects_()
{
	window_.setKeyRepeatEnabled(false);
	window_.setVerticalSyncEnabled(true);

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	scoreText_.setString("0");
	scoreText_.setFont(font_);
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
			std::unique_ptr<Ball> pBall	(new Ball(ecs_,
				                                  Vector2f(20.f, 580.f),
				                                  ballRadius_, ballMass_, ballColor_));
			auto entBall = pBall->getLabel();
			auto velComp = dynCast<ecs::Velocity>
				(ecs_.getComponent(entBall, ecs::Component::Velocity));
			assert(velComp);

			Vector2f force = Vector2f(20.f, 580.f) - Vector2f(sf::Mouse::getPosition(window_));

			velComp->velocity_ -= 3.f * force / ballMass_;
			balls_.push_back(std::move(pBall));
		}

		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Right)			
		{
			std::unique_ptr<Wall> pRect(new Wall(ecs_,
			                                     Vector2f(sf::Mouse::getPosition(window_)),
			                                     Vector2f(15.f, 75.f),
			                                     sf::Color::Blue));
			rects_.push_back(std::move(pRect));
		}

		else if (event.type == sf::Event::MouseButtonPressed &&
		         event.mouseButton.button == sf::Mouse::Middle)			
		{
			std::unique_ptr<Wall> pRect(new Wall(ecs_,
			                                     Vector2f(sf::Mouse::getPosition(window_)),
			                                     Vector2f(800.f, 15.f),
			                                     sf::Color::Blue));
			rects_.push_back(std::move(pRect));
		}

	}
}

void Application::update(sf::Time dt)
{
	ecs_.update(dt);
	gameWorld_.update(dt);

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
	for (auto& rect : rects_)
	{
		rect->update(dt);
	}
	
	// score_ += engine_.getTrackedCollisions().size();
	// std::stringstream ss;
	// ss << score_;
	// scoreText_.setString(ss.str());
}

void Application::render()
{
	window_.clear();

	gameWorld_.draw();

	for (const auto& ball : balls_)
	{
		window_.draw(*ball);
	}
	for (const auto& rect : rects_)
	{
		window_.draw(*rect);
	}	

	window_.draw(scoreText_);
	window_.display();
}
