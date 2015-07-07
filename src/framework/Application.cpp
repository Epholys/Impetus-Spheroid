#include "framework/Assertion.hpp"
#include "framework/Application.hpp"


//-----------------------------------------------------------------------------
// *** Constructor Constants: ***

namespace
{
	const Vector2u WINDOW_SIZE {800, 600};

	const unsigned int GL_DEPTH = 0;
	const unsigned int GL_STENCIL = 0;
	const unsigned int GL_ANTI_ALIASING = 4;

	// False in general, but for now it doesn't impact the gameplay
	const bool IS_AZERTY = true;

	const std::string CROSS_HAIR_LOCATION ("media/sprites/Cross-hair.png");
}


//-----------------------------------------------------------------------------
// *** Contructor: ***

Application::Application()
	: window_(sf::VideoMode(WINDOW_SIZE.x, WINDOW_SIZE.y),
	          "Impetus Spheroid",
	          sf::Style::Resize | sf::Style::Close,
	          sf::ContextSettings(GL_DEPTH, GL_STENCIL, GL_ANTI_ALIASING))
	, globalTransform_(sf::Transform::Identity)
	, metaData_(WINDOW_SIZE, IS_AZERTY)
	, lastGameData_()
	, fontHolder_()
	, textureHolder_()
	, stack_(State::Context(window_, fontHolder_, textureHolder_, metaData_, lastGameData_))
	, crossHairActivated_(false)
	, crossHairTexture_()
	, crossHair_()
{
	window_.setFramerateLimit(0); // Unlimited Framerate
	window_.setVerticalSyncEnabled(false);

	if(!DataSaver::retrieveDatas(metaData_))
	{
		DataSaver::salvageDatas();
		DataSaver::retrieveDatas(metaData_);
	}

	stack_.registerState<StateGame>(StateID::Game);
	stack_.registerState<StateOver>(StateID::GameOver);
	stack_.registerState<StatePause>(StateID::Pause);
	stack_.registerState<StateMarket>(StateID::Market);

////////////////////////////////////////////////////////////////////////////////	
	// stack_.pushState(StateID::Game);
	stack_.pushState(StateID::Market);
////////////////////////////////////////////////////////////////////////////////
	
	// gui::TextureGenerator::createMarketButtons(metaData_.inventory.getKeys());
	
	fontHolder_.load(FontID::ForcedSquare, "./media/font/FORCEDSQUARE.ttf");

	textureHolder_.load(TextureID::CrossHair, CROSS_HAIR_LOCATION);
	crossHair_.setTexture(textureHolder_.get(TextureID::CrossHair));
	centerOrigin(crossHair_);
}


//-----------------------------------------------------------------------------
// *** Main loop: ***

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

void Application::update(sf::Time dt)
{
	stack_.update(dt);

	if(typeid(*stack_.upperState()) == typeid(StateGame))
	{
		crossHairActivated_ = true;
		window_.setMouseCursorVisible(false);
	}
	else
	{
		crossHairActivated_ = false;
		window_.setMouseCursorVisible(true);
	}

	const int DEGREE_PER_SECONDS = 90;
	crossHair_.rotate(DEGREE_PER_SECONDS * dt.asSeconds());
}

void Application::render()
{
	const sf::Color BACKGROUND_COLOR (230, 230, 230);

	window_.clear(BACKGROUND_COLOR);

	sf::RenderStates states;
	states.transform *= globalTransform_;
	stack_.draw(states);

	if(crossHairActivated_)
	{
		window_.draw(crossHair_);
	}
	
	window_.display();
}

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
			Vector2u newWindowSize {event.size.width, event.size.height};
			handleResizing(newWindowSize);
		}
		else if(event.type == sf::Event::MouseMoved)
		{
			Vector2i mousePosition {event.mouseMove.x, event.mouseMove.y};

			crossHair_.setPosition(Vector2f(mousePosition));

			correctMouseCoordinate(mousePosition);
			event.mouseMove.x = mousePosition.x;
			event.mouseMove.y = mousePosition.y;
		}
		else if(event.type == sf::Event::MouseButtonPressed ||
		        event.type == sf::Event::MouseButtonReleased )
		{
			Vector2i mousePosition {event.mouseButton.x, event.mouseButton.y};
			correctMouseCoordinate(mousePosition);
			event.mouseButton.x = mousePosition.x;
			event.mouseButton.y = mousePosition.y;
		}

		stack_.handleEvent(event);
	}
}


//-----------------------------------------------------------------------------
// *** Helper functions: ***

void Application::handleResizing(Vector2u newWindowSize)
{
	globalTransform_ = sf::Transform::Identity;

	float originalRatio = WINDOW_SIZE.x / float(WINDOW_SIZE.y);
	float newRatio = newWindowSize.x / float(newWindowSize.y);

	if(newRatio > originalRatio)
	{
		float scale = newWindowSize.y / float(WINDOW_SIZE.y);
		float margin = std::abs(WINDOW_SIZE.x * scale - newWindowSize.x) / 2.f;
		globalTransform_.translate(margin, 0.f).scale(scale, scale);
		
		crossHair_.setScale(scale, scale);
	}

	else
	{
		float scale = newWindowSize.x / float(WINDOW_SIZE.x);
		float margin = std::abs(WINDOW_SIZE.y * scale - newWindowSize.y) / 2.f;
		globalTransform_.translate(0.f, margin).scale(scale, scale);

		crossHair_.setScale(scale, scale);
	}

	window_.setView(
		sf::View(
			sf::FloatRect(0, 0, newWindowSize.x, newWindowSize.y)));
}

void Application::correctMouseCoordinate(Vector2i& mousePosition)
{
	mousePosition = 
		Vector2i(globalTransform_.getInverse().transformPoint(Vector2f(mousePosition)));
}
