#include "core/Market.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto datas = genMarketData();
}


//-----------------------------------------------------------------------------

namespace
{
	Vector2f COIN_LOST_OFFSET (10.f, 0.f);

}


//-----------------------------------------------------------------------------

Market::Market(State::Context context)
	: inventory_(context.metaData->inventory)
	, menu_()
	, font_()
	, coinsText_()
	, coinsLost_()
	, coinsLostTransition_(nullptr, gui::Transition::Linear, Vector2f(), Vector2f(), Time())
	, context_(context)
{
	initText();
	initGUI();
}

void Market::initText()
{
	const sf::Vector2f TEXT_POSITION (context_.originalWindowSize.x / 2.f, 50);
	
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	coinsText_.setFont(font_);
	updateCoinsText();
	coinsText_.setColor(sf::Color::Yellow);
	centerOrigin(coinsText_);
	coinsText_.setPosition(TEXT_POSITION);

	coinsLost_.setFont(font_);
	coinsLost_.setColor(sf::Color(255, 200, 0));
	centerOrigin(coinsLost_);
	coinsLost_.setPosition(coinsText_.findCharacterPos(100) + COIN_LOST_OFFSET);
}

void Market::initGUI()
{
	gui::Menu::SPtr menu = std::make_shared<gui::Menu>
		(gui::Menu::Vertical, "Welcome to the Market!");


	const sf::Vector2u BUTTON_SIZE (600, 50);
	const sf::Vector2u BEGIN_MIDDLE (400, 100);
	const int SPACE_SIZE = 10;
	const auto KEYS = inventory_.getKeys();
	int buttonPosition = 0;
	
	for(auto it=datas.begin(); it!=datas.end(); ++it)
	{
		gui::Button::SPtr button = std::make_shared<gui::Button>();
		sf::Texture texture;
		texture.loadFromFile(it->texturePath);

		button->setCallback(std::bind(&Market::buy, this, it->id, it->number, it->price));
		button->setKey(KEYS.at(it->id));
		button->setTexture(texture);
		button->setOrigin(BUTTON_SIZE.x / 2.f, 0.f);
		button->setPosition(BEGIN_MIDDLE.x, BEGIN_MIDDLE.y + buttonPosition * (BUTTON_SIZE.y + SPACE_SIZE));

		menu->pack(button);
		
		++buttonPosition;
	}
	
	menu->select();
	
	menu_ = menu;
}


//-----------------------------------------------------------------------------

void Market::update(Time dt)
{
	coinsLostTransition_.update(dt);
}


//-----------------------------------------------------------------------------

void Market::buy(PowerUpID::ID id, int number, int price)
{
	if(price <= inventory_.getCoins())
	{
		inventory_.removeCoins(price);
		inventory_.increment(id, number);
		updateCoinsLoss(price);
		updateCoinsText();
		DataSaver::saveDatas(*context_.metaData);
	}
}

void Market::updateCoinsText()
{
	std::stringstream ss;
	ss << "COINS: " << inventory_.getCoins();
	coinsText_.setString(ss.str());
}

void Market::updateCoinsLoss(int price)
{
	std::stringstream ss;
	ss << "-" << price;
	coinsLost_.setString(ss.str());
	
	coinsLost_.setPosition(coinsText_.findCharacterPos(100) + COIN_LOST_OFFSET);
	Vector2f coinsPos = Vector2f(coinsLost_.getPosition());
	coinsLostTransition_ = gui::Transition(&coinsLost_, gui::Transition::Linear, coinsPos, coinsPos + Vector2f(30.f, 0.f), seconds(1.f));
}

//-----------------------------------------------------------------------------

void Market::handleEvent(const sf::Event& event)
{
	menu_->handleEvent(event);
}

void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*menu_, states);
	target.draw(coinsText_, states);
	target.draw(coinsLost_, states);
}
