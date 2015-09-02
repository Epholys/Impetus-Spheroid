#include "framework/StateMarket.hpp"


//-----------------------------------------------------------------------------

StateMarket::StateMarket(StateStack& stack, Context context)
	: State(stack, context)
	, market_(context)
	, retryButton_()
	, menuButton_()
{
	const Vector2f TEXTURE_SIZE {650.f, 50.f};
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
	                     "Shoot Again!",
						 context.fonts->get(FontID::ForcedSquare),
	                     30,
	                     sf::Color::Black);
	retryButton_.setKey(sf::Keyboard::Space);
	retryButton_.move(RETRY_POS);

	menuButton_.setCallback(
		[this]()
		{
			requestStackPush(StateID::Menu);
		});
	menuButton_.setDefaultTexture();
	menuButton_.setLabel(gui::Button::Middle,
	                      "Menu",
	                      context.fonts->get(FontID::ForcedSquare),
	                      30,
	                      sf::Color::Black);
	menuButton_.move(RETRY_POS + Vector2f(0.f, 60.f));
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
	window->draw(menuButton_, states);
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
	menuButton_.handleEvent(event);
	market_.handleEvent(event);

	return false;
}
