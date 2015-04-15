#include "framework/Assertion.hpp"
#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: window_(sf::VideoMode(800, 600),
	          "Impetus Spheroid",
	          sf::Style::Titlebar | sf::Style::Close,
	          sf::ContextSettings(0, 0, 4))
	, datas_()
	, stack_(State::Context(window_, datas_))
{
	window_.setFramerateLimit(0);
	window_.setVerticalSyncEnabled(false);

	datas_.inventory.switchKeyboard(true);
	if(!DataSaver::retrieveDatas(datas_))
	{
		std::cout << "NEW FILE\n";
		DataSaver::makeDefaultFile();
		DataSaver::retrieveDatas(datas_);
	}
	

	stack_.registerState<StateGame>(StateID::Game);
	stack_.registerState<StateOver>(StateID::GameOver);
	stack_.registerState<StatePause>(StateID::Pause);
	
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
		stack_.handleEvent(event);

		if (event.type == sf::Event::Closed)
		{
			window_.close();
		}
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
