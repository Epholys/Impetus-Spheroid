#include "core/Market.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto datas = genMarketData();
}


//-----------------------------------------------------------------------------

namespace
{
	const int UNLOCKING_COEFF = 3;

	const sf::Vector2u BUTTON_SIZE (600, 50);
		
	Vector2f COIN_LOST_OFFSET (10.f, 0.f);
	sf::Color COIN_LOST_COLOR (255, 200, 0);
}


//-----------------------------------------------------------------------------

Market::Market(State::Context context)
	: inventory_(context.metaData->inventory)
	, isUnlocked_(context.metaData->isPowerUpUnlocked)
	, menu_()
	, buttons_()
	, coinsText_()
	, coinsLost_()
	, coinsLostTransition_(nullptr,
	                       gui::Transition::Linear,
	                       gui::TransformData(),
	                       gui::TransformData(),
	                       Time())
	, coinsLostFadeOut_{nullptr, Time(), Time()}
	, context_(context)
{
	sf::Font& font (context.fonts->get(FontID::ForcedSquare));
	initText(font);
	initGUI(font);
}

void Market::initText(sf::Font& font)
{
	const sf::Vector2f TEXT_POSITION (context_.originalWindowSize.x / 2.f, 50);
	
	coinsText_.setFont(font);
	updateCoinsText();
	coinsText_.setColor(sf::Color::Yellow);
	centerOrigin(coinsText_);
	coinsText_.setPosition(TEXT_POSITION);

	coinsLost_.setFont(font);
	coinsLost_.setColor(COIN_LOST_COLOR);
	centerOrigin(coinsLost_);
	coinsLost_.setPosition(coinsText_.findCharacterPos(100) + COIN_LOST_OFFSET);
}

void Market::initGUI(sf::Font& font)
{
	gui::Menu::SPtr menu = std::make_shared<gui::Menu>(gui::Menu::Vertical);


	const sf::Vector2u BEGIN_MIDDLE (400, 100);
	const int SPACE_SIZE = 10;
	const auto KEYS = inventory_.getKeys();
	int buttonPosition = 0;
	
	for(auto it=datas.begin(); it!=datas.end(); ++it)
	{
		gui::Button::SPtr button = std::make_shared<gui::Button>();
		button->setKey(KEYS.at(it->id));
		button->setOrigin(BUTTON_SIZE.x / 2.f, 0.f);
		button->setPosition(BEGIN_MIDDLE.x, BEGIN_MIDDLE.y + buttonPosition * (BUTTON_SIZE.y + SPACE_SIZE));

		sf::Texture texture;
		if(isUnlocked_[it->id])
		{
			texture.loadFromFile(it->texturePath);
		}
		else
		{
			createBlankButton(texture, it->price * UNLOCKING_COEFF, font);
		}
		button->setTexture(texture);
			
		button->setCallback(std::bind(&Market::buy, this, it->id, it->number, it->price));
		
		menu->pack(button);
		buttons_[it->id] = button;
		
		++buttonPosition;
	}
	
	gui::MenuMeta::SPtr metaMenu = std::make_shared<gui::MenuMeta>(gui::Menu::Horizontal);
	metaMenu->pack(menu, "PowerUp");

	metaMenu->select();
	
	menu_ = metaMenu;
}


//-----------------------------------------------------------------------------

void Market::update(Time dt)
{
	coinsLostTransition_.update(dt);
	coinsLostFadeOut_.update(dt);
}


//-----------------------------------------------------------------------------

void Market::buy(PowerUpID::ID id, int number, int price)
{
	if(isUnlocked_[id] && price <= inventory_.getCoins())
	{
		inventory_.removeCoins(price);
		inventory_.increment(id, number);
		updateCoinsLoss(price);
		updateCoinsText();
		DataSaver::saveDatas(*context_.metaData);
	}
	else if(!isUnlocked_[id] && price * UNLOCKING_COEFF <= inventory_.getCoins())
	{
		inventory_.removeCoins(price);
		isUnlocked_[id] = true;
		sf::Texture texture;
		assert(texture.loadFromFile(datas[id].texturePath));
		buttons_[id]->setTexture(texture);
		updateCoinsLoss(price * UNLOCKING_COEFF);
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
	coinsLost_.setColor(COIN_LOST_COLOR);
	Vector2f coinsPos = Vector2f(coinsLost_.getPosition());

	coinsLostTransition_ = gui::Transition(&coinsLost_,
	                                       gui::Transition::Linear,
	                                       gui::TransformData(coinsPos),
	                                       gui::TransformData(coinsPos + Vector2f(30.f, 0.f)),
	                                       seconds(1.f)); 
	coinsLostFadeOut_ = gui::FadeOut<sf::Text> {&coinsLost_, seconds(1.f), Time()};
}

//-----------------------------------------------------------------------------

void Market::handleEvent(const sf::Event& event)
{
	menu_->handleEvent(event);
}


//-----------------------------------------------------------------------------

void Market::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*menu_, states);
	target.draw(coinsText_, states);
	target.draw(coinsLost_, states);
}

void Market::createBlankButton(sf::Texture& texture, int price, sf::Font& font)
{
	sf::RenderTexture rTexture;
	assert(rTexture.create(BUTTON_SIZE.x + BUTTON_SIZE.y, BUTTON_SIZE.y));
	
	sf::Text priceText(std::to_string(price), font);
	priceText.setColor(sf::Color::Yellow);
	priceText.setCharacterSize(BUTTON_SIZE.y * 0.7f);
	centerOrigin(priceText);
	priceText.setPosition(Vector2f(BUTTON_SIZE) / 2.f);

	sf::RectangleShape rect (Vector2f(BUTTON_SIZE.y, BUTTON_SIZE.y));
	rect.setFillColor(sf::Color::Black);
	rect.setPosition(BUTTON_SIZE.x, 0.f);

	rTexture.clear(sf::Color(230,230,230));
	rTexture.draw(priceText);
	rTexture.draw(rect);
	rTexture.display();

	texture = rTexture.getTexture();
}

