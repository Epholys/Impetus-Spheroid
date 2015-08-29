#include "framework/StateMarket.hpp"


//-----------------------------------------------------------------------------

StateMarket::StateMarket(StateStack& stack, Context context)
	: State(stack, context)
	, market_(context)
	, retryButton_()
{
	const Vector2f TEXTURE_SIZE {600.f, 50.f};
	const Vector2f RETRY_POS ((context.originalWindowSize.x - TEXTURE_SIZE.x) / 2, 4 * context.originalWindowSize.y / 5);
	retryButton_.setCallback(
		[this]()
		{
			requestStackClear();
			requestStackPush(StateID::Game);
			DataSaver::saveDatas(*context_.metaData);
		});

	retryButton_.setDefaultTexture();
	retryButton_.setLabel(gui::Button::Middle,
	                     "RETRY",
						 context.fonts->get(FontID::ForcedSquare),
	                     30,
	                     sf::Color::Black);
	retryButton_.setKey(sf::Keyboard::Space);
	retryButton_.move(RETRY_POS);
}

StateMarket::~StateMarket()
{
}


//-----------------------------------------------------------------------------

void StateMarket::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	window->draw(rect);

	window->draw(retryButton_, states);
	window->draw(market_, states);
}

bool StateMarket::update(Time dt)
{
	market_.update(dt);
	return true;
}

bool StateMarket::handleInput(const sf::Event& event)
{
	retryButton_.handleEvent(event);
	market_.handleEvent(event);

	return false;
}
