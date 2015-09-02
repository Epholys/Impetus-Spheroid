#include "framework/StateMenu.hpp"
#include "utility/utility.hpp"


//-----------------------------------------------------------------------------

StateMenu::StateMenu(StateStack& stack, Context context)
	: State(stack, context),
	  menu_(gui::Menu::Vertical),
	  logo_()
{
	const float SPACE_SIZE = 60.f;
	int i = 0;
	
	gui::Button::SPtr resumeButton = genStandardButton("Resume");
	resumeButton->setCallback([this](){requestStackPop();});
	resumeButton->setKey(sf::Keyboard::Space);
	resumeButton->move(0.f, SPACE_SIZE * i); ++i;
	menu_.pack(resumeButton);

	gui::Button::SPtr newGameButton = genStandardButton("Restart Shooting!");
	newGameButton->setCallback(
		[this]()
		{
			DataSaver::saveDatas(*context_.metaData);
			requestStackClear();
			requestStackPush(StateID::Game);
		});
	newGameButton->move(0.f, SPACE_SIZE * i); ++i;
	menu_.pack(newGameButton);

	gui::Button::SPtr marketButton = genStandardButton("Upgrades");
	marketButton->setCallback(
		[this]()
		{
			DataSaver::saveDatas(*context_.metaData);
			requestStackClear();
			requestStackPush(StateID::Market);
		});
	marketButton->move(0.f, SPACE_SIZE * i); ++i;
	menu_.pack(marketButton);

	// gui::Button::SPtr creditButton = genStandardButton("Credit");
	// creditButton->setCallback([this](){});
	// creditButton->move(0.f, SPACE_SIZE * i); ++i;
	// menu_.pack(creditButton);

	gui::Button::SPtr exitButton = std::make_shared<gui::Button>();
	exitButton->setDefaultTexture();
	exitButton->setAlpha(100);
	exitButton->setLabel(gui::Button::Middle, "Exit", context_.fonts->get(FontID::ForcedSquare), 30, sf::Color::Red);
	exitButton->setCallback(
		[this]()
		{
			DataSaver::saveDatas(*context_.metaData);
			context_.window->close();
		});
	exitButton->move(0.f, SPACE_SIZE * i); ++i;
	menu_.pack(exitButton);

	const Vector2f MENU_POS (75.f, 175.f);
	menu_.setPosition(MENU_POS);
	menu_.select();


	context.textures->load(TextureID::Logo, "./media/sprites/Logo.png");
	logo_.setTexture(context.textures->get(TextureID::Logo));
	centerOrigin(logo_);
	logo_.setPosition(Vector2f(400.f, 75.f));
}

gui::Button::SPtr StateMenu::genStandardButton(const std::string& label) const
{
	gui::Button::SPtr button = std::make_shared<gui::Button>();
	button->setDefaultTexture();
	button->setAlpha(100);
	button->setLabel(gui::Button::Middle, label, context_.fonts->get(FontID::ForcedSquare));
	return button;
}

StateMenu::~StateMenu()
{
}


//-----------------------------------------------------------------------------


void StateMenu::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(25,25,25));
	window->draw(rect);

	window->draw(logo_, states);
	window->draw(menu_, states);
}

bool StateMenu::update(Time)
{
	return false;
}

bool StateMenu::handleInput(const sf::Event& event)
{
	menu_.handleEvent(event);
	
	return false;
}
