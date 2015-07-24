#include "core/Market.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto marketDatas = genMarketData();
	auto improvementDatas = genImprovementData();
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
	, improvementValue_(context.metaData->improvementValue)
	, needUpdate_(false)
	, menu_()
	, powerUpButtons_()
	, improvementButtons_()
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
	const sf::Vector2f TEXT_POSITION (context_.originalWindowSize.x / 2.f, 15.f);
	
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
	const sf::Vector2u BEGIN_MIDDLE ((800.f - BUTTON_SIZE.x) / 2.f, 45.f);
	
	gui::Menu::SPtr puMenu = std::make_shared<gui::Menu>(gui::Menu::Vertical);
	initPowerUps(puMenu, font);
   	
	gui::Menu::SPtr improvMenu = std::make_shared<gui::Menu>(gui::Menu::Vertical);
	initImprovement(improvMenu, font);

	gui::MenuMeta::SPtr metaMenu = std::make_shared<gui::MenuMeta>(gui::Menu::Horizontal);
	metaMenu->move(Vector2f(BEGIN_MIDDLE));
	metaMenu->pack(puMenu, "PowerUp");
	metaMenu->pack(improvMenu, "Improvements");
	
	metaMenu->select();
	
	menu_ = metaMenu;
}

void Market::initPowerUps(gui::Menu::SPtr menu, sf::Font& font)
{
	const sf::Vector2u BEGIN_MIDDLE ((800.f - BUTTON_SIZE.x) / 2.f, 100);
	const int SPACE_SIZE = 10;
	const auto KEYS = inventory_.getKeys();
	int buttonPosition = 1;
	for(auto it=marketDatas.begin(); it!=marketDatas.end(); ++it)
	{
		gui::Button::SPtr button = std::make_shared<gui::Button>();
		button->setKey(KEYS.at(it->id));
		button->move(0.f, buttonPosition * (BUTTON_SIZE.y + SPACE_SIZE));

		if(isUnlocked_[it->id])
		{
			sf::Texture texture;
			texture.loadFromFile(it->texturePath);
			button->setTexture(texture);
		}
		else
		{
			button->setDefaultTexture();
			std::stringstream ss;
			ss << it->price * UNLOCKING_COEFF;
			button->setLabel(gui::Button::Middle, ss.str(), font, BUTTON_SIZE.y * 0.7f, sf::Color::Yellow);
		}
			
		button->setCallback(std::bind(&Market::buyPowerUp, this, it->id, it->number, it->price));
		
		menu->pack(button);
		powerUpButtons_[it->id] = button;
		
		++buttonPosition;
	}
}

void Market::initImprovement(gui::Menu::SPtr menu, sf::Font& font)
{
	const sf::Vector2u BEGIN_MIDDLE ((800.f - BUTTON_SIZE.x) / 2.f, 100);
	const int SPACE_SIZE = 10;
	int buttonPosition = 1;
	for(unsigned int i=0; i<improvementValue_.size(); ++i)
	{
		gui::Button::SPtr button = std::make_shared<gui::Button>();
		button->move(0.f, buttonPosition * (BUTTON_SIZE.y + SPACE_SIZE));

		button->setDefaultTexture();

		const auto& data = improvementDatas[i];
		std::stringstream ss;
		ss << improvementValue_[i];
		button->setLabel(gui::Button::Middle,
		                 data.description,
		                 font,
		                 BUTTON_SIZE.y * 0.4f,
		                 sf::Color::Black);
		sf::Text& labelMid = button->getLabel(gui::Button::Middle);
		Vector2f position (labelMid.findCharacterPos(0));
		button->setLabel(gui::Button::Left,
		                 ss.str(),
		                 font,
		                 30,
		                 sf::Color::Green,
		                 Vector2f(position.x - BUTTON_SIZE.y * 0.4f, 0.f));
		sf::Text& labelLeft = button->getLabel(gui::Button::Left);
		sf::FloatRect bounds = labelLeft.getGlobalBounds();
		labelLeft.move(-bounds.width / 2.f, 0.f);
		
		int price = data.basePrice + (improvementValue_[i] - data.baseValue) * data.priceIncrement / data.increment;
		ss.str("");
		ss << price;
		button->setLabel(gui::Button::Right,
		                 ss.str(),
		                 font,
		                 30,
		                 sf::Color::Yellow,
		                 Vector2f(-10.f,0.f));
		                 
		button->setCallback(
			[this, data, price, i]()
			{
				buyImprovement(ImprovementID::ID(i), data.increment, price);
			});
		
		menu->pack(button);
		
		++buttonPosition;
	}
}

//-----------------------------------------------------------------------------

void Market::update(Time dt)
{
	coinsLostTransition_.update(dt);
	coinsLostFadeOut_.update(dt);

	if(needUpdate_)
	{
		menu_->unpack("Improvements");
		gui::Menu::SPtr improvMenu = std::make_shared<gui::Menu>(gui::Menu::Vertical);
		initImprovement(improvMenu, context_.fonts->get(FontID::ForcedSquare));
		menu_->pack(improvMenu, "Improvements");
		needUpdate_ = false;
	}
}


//-----------------------------------------------------------------------------

void Market::buyPowerUp(PowerUpID::ID id, int number, int price)
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
		inventory_.removeCoins(price * UNLOCKING_COEFF);
		isUnlocked_[id] = true;
		sf::Texture texture;
		assert(texture.loadFromFile(marketDatas[id].texturePath));
		powerUpButtons_[id]->setTexture(texture);
		powerUpButtons_[id]->removeLabel(gui::Button::Middle);
		updateCoinsLoss(price * UNLOCKING_COEFF);
		updateCoinsText();
		DataSaver::saveDatas(*context_.metaData);
	}
}

void Market::buyImprovement(ImprovementID::ID id, int increment, int price)
{
	if(price < inventory_.getCoins())
	{
		inventory_.removeCoins(price);
		improvementValue_[id] += increment;
		updateCoinsLoss(price);
		updateCoinsText();
		DataSaver::saveDatas(*context_.metaData);
		needUpdate_ = true;
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
