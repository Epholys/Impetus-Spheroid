#include "framework/Assertion.hpp"
#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: windowSize_(800, 600) 
	, window_(sf::VideoMode(windowSize_.x, windowSize_.y),
	          "Impetus Spheroid",
	          sf::Style::Resize | sf::Style::Close,
	          sf::ContextSettings(0, 0, 4))
	, globalTransform_(sf::Transform::Identity)
	, datas_()
	, stack_(State::Context(window_, globalTransform_, datas_))
{
	window_.setFramerateLimit(0);
	window_.setVerticalSyncEnabled(false);

	datas_.inventory.switchKeyboard(true);
	if(!DataSaver::retrieveDatas(datas_))
	{
		DataSaver::makeDefaultFile();
		DataSaver::retrieveDatas(datas_);
	}
	

	stack_.registerState<StateGame>(StateID::Game);
	stack_.registerState<StateOver>(StateID::GameOver);
	stack_.registerState<StatePause>(StateID::Pause);
	stack_.registerState<StateMarket>(StateID::Market);
	
	stack_.pushState(StateID::Game);
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

		else if (event.type == sf::Event::Resized)
		{
			globalTransform_ = sf::Transform::Identity;
			float originalRatio = windowSize_.x / float(windowSize_.y);
			float newRatio = event.size.width / float(event.size.height);
			if(newRatio > originalRatio)
			{
				float scale = event.size.height / float(windowSize_.y);
				float margin = std::abs(float(windowSize_.x) * scale - float(event.size.width)) / 2.f;
				globalTransform_.translate(margin, 0.f);
				globalTransform_.scale(scale, scale);
			}
			else
			{
				float scale = event.size.width / float(windowSize_.x);
				float margin = std::abs(float(windowSize_.y) * scale - float(event.size.height)) / 2.f;
				globalTransform_.translate(0.f, margin);
				globalTransform_.scale(scale, scale);
			}

			window_.setView(
				sf::View(
					sf::FloatRect(0, 0, event.size.width, event.size.height)));
		}

		stack_.handleEvent(event);
	}
}

void Application::update(sf::Time dt)
{
	stack_.update(dt);
}

void Application::render()
{
	window_.clear(sf::Color(230,230,230));

	stack_.draw();

	window_.display();
}
