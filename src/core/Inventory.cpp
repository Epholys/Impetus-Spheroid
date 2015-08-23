#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

Inventory::Inventory()
	: Inventory(Vector2u(800, 600), true)
{
}

Inventory::Inventory(Vector2u originalSize,
                     bool isAzerty)
	: originalSize_(originalSize)
	, font_()
	, coins_(0)
	, keyBindings_()
	, powerUpTable_()
	, world_(nullptr)
{
	initKeyBinding(isAzerty);
	genPowerUps(powerUpTable_);
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	const Vector2f COINS_POSITION (710.f, 40.f);
	coinsText_.setFont(font_);
	coinsText_.setPosition(COINS_POSITION);
	coinsText_.setString(toString(coins_));
	coinsText_.setColor(sf::Color(255,165,0));
}

Inventory::~Inventory()
{
}


//-----------------------------------------------------------------------------

void Inventory::switchKeyboard(bool isAzerty)
{
	initKeyBinding(isAzerty);
}

void Inventory::initKeyBinding(bool isAzerty)
{
	using namespace PowerUpID;
	
	if(isAzerty)
	{
		keyBindings_[sf::Keyboard::A] = GhostBall;
		keyBindings_[sf::Keyboard::Z] = NoGravBall;
		keyBindings_[sf::Keyboard::W] = PointMultiplier;
	}
	else
	{
		keyBindings_[sf::Keyboard::Q] = GhostBall;
		keyBindings_[sf::Keyboard::W] = NoGravBall;
		keyBindings_[sf::Keyboard::Z] = PointMultiplier;
	}

	keyBindings_[sf::Keyboard::E] = CancelEvents;
	keyBindings_[sf::Keyboard::S] = AddTime;
	keyBindings_[sf::Keyboard::D] = AddTarget;
	keyBindings_[sf::Keyboard::X] = BallTouchDouble;
	keyBindings_[sf::Keyboard::C] = AddCannon;
	
	for(auto it = keyBindings_.begin(); it != keyBindings_.end(); ++it) 
	{
		powerUpTable_[it->second].key = it->first;
	}
}

void Inventory::addWorld(World* world)
{
	world_ = world;
}

void Inventory::removeWorld()
{
	// Do some clean-up
	for (auto& entry : powerUpTable_)
	{
		auto& powerUp = entry.second.powerUp;
		if(powerUp->getType() == PowerUpType::Toogle &&
		   world_)
		{
			// Launch the deactivate_() of powerUpToogle, if it is active
			auto powerUpToogle = std::dynamic_pointer_cast<PowerUpToogle>(powerUp);
			if(powerUpToogle && powerUpToogle->isActivated())
			{
				powerUp->apply(*world_);
			}
		}
	}

	world_ = nullptr;
}


//-----------------------------------------------------------------------------

void Inventory::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		if(!world_)
			return;

		auto gotId = keyBindings_.find(event.key.code);
		if(gotId == keyBindings_.end())
			return;
		
		auto id = gotId->second;
		auto gotEntry = powerUpTable_.find(id);
		if(gotEntry == powerUpTable_.end())
			return;

		auto& powerUp = gotEntry->second.powerUp;
		int& count = gotEntry->second.stock;

		if(count == 0)
			return;

		powerUp->apply(*world_);

		if(powerUp->getType() == PowerUpType::Modifier)
		{
			--count;
		}
	}
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if(!world_)
		return;
	
	const std::string str = toString(coins_);
	coinsText_.setString(str);
	target.draw(coinsText_, states);
	
	const Vector2u POWERUP_ICON_SIZE (20, 20);
	const Vector2u SPACE_BEETWEEN_ICONS (10, 30);
	const int POWERUP_PER_LINE = 10;
	auto targetSize = originalSize_;

	int nPowerUp = std::count_if(powerUpTable_.begin(),
	                             powerUpTable_.end(),
//	                             [](const auto& pair)
	                             [](const std::pair<PowerUpID::ID, PowerUpEntry> entry)
	                             {
		                             return entry.second.stock > 0;
	                             });

	if(nPowerUp == 0)
		return;

	int xCoord = targetSize.x / 2 - (POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS.x) * (POWERUP_PER_LINE) / 2;
	int yCoord = targetSize.y * 0.90f - (POWERUP_ICON_SIZE.y + SPACE_BEETWEEN_ICONS.y) * ((nPowerUp-1) / POWERUP_PER_LINE);

	int nTextureDrawn=0;
	for(auto it = powerUpTable_.begin(); it != powerUpTable_.end(); ++it)
	{
		if(it->second.stock > 0) 
		{
			sf::Text num;
			num.setFont(font_);
			num.setString(toString(it->second.stock));
			num.setPosition(xCoord, yCoord + POWERUP_ICON_SIZE.y / 1.5);
			num.setCharacterSize(20);
			num.setColor(sf::Color::Black);

			sf::Sprite sprite;
			sprite.setTexture(it->second.texture);
			sprite.setPosition(xCoord, yCoord);

			sf::Text key;
			key.setFont(font_);
			key.setString(toString(it->second.key));
			key.setPosition(xCoord, yCoord - POWERUP_ICON_SIZE.y);
			key.setCharacterSize(20);
			key.setColor(sf::Color::Black);

			target.draw(key, states);
			target.draw(sprite, states);
			target.draw(num, states);

			auto pPowerupToogle = std::dynamic_pointer_cast<PowerUpToogle>(it->second.powerUp);
			if(pPowerupToogle && pPowerupToogle->isActivated())
			{
				const sf::Vector2u OFFSET (2, 2);
				sf::RectangleShape rect (Vector2f(POWERUP_ICON_SIZE.x + 2 * OFFSET.x, POWERUP_ICON_SIZE.y + 25 + 2 * OFFSET.y));
				rect.setFillColor(sf::Color(0,0,0,0));
				rect.setOutlineThickness(2.f);
				rect.setOutlineColor(sf::Color::Black);
				rect.setPosition(Vector2f(xCoord, yCoord) - Vector2f(OFFSET) - Vector2f(0.f, 10.f));
				target.draw(rect, states);
			}
			
			++nTextureDrawn;
		
			int nextPositionInLine = nTextureDrawn % POWERUP_PER_LINE;
			if(nextPositionInLine != 0)
			{
				xCoord += POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS.x;
			}
			else
			{
				xCoord = targetSize.x / 2 - (POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS.x) * (POWERUP_PER_LINE) / 2;
				yCoord += POWERUP_ICON_SIZE.y + SPACE_BEETWEEN_ICONS.y;
			}
		}
	}
}

		
//-----------------------------------------------------------------------------

bool Inventory::setPowerUp(PowerUpID::ID id, int value)
{
	if(value < 0)
		return false;

	auto gotEntry = powerUpTable_.find(id);
	if(gotEntry == powerUpTable_.end())
		return false;

	int& count = gotEntry->second.stock;
	count = value;

	return true;
}

int Inventory::getPowerUp(PowerUpID::ID id) const
{
	auto gotEntry = powerUpTable_.find(id);
	if(gotEntry == powerUpTable_.end())
		return -1;

	return gotEntry->second.stock;
}


bool Inventory::decrement(PowerUpID::ID id)
{
	auto gotEntry = powerUpTable_.find(id);
	if(gotEntry == powerUpTable_.end())
		return false;

	auto& powerUp = gotEntry->second.powerUp;
	int& count = gotEntry->second.stock;
	// Absorb the error
	if(count == 0)
		return false;

	--count;

	if(powerUp->getType() == PowerUpType::Toogle &&
	   count == 0 &&
	   world_)
	{
		// Launch the deactivate_() of powerUpToogle, if it is active
		auto powerUpToogle = std::dynamic_pointer_cast<PowerUpToogle>(powerUp);
		if(powerUpToogle && powerUpToogle->isActivated())
		{
			powerUp->apply(*world_);
		}
	}
	
	return true;
}

void Inventory::increment(PowerUpID::ID id, int value)
{
	auto gotEntry = powerUpTable_.find(id);
	if(gotEntry == powerUpTable_.end())
		return;
	
	int& count = gotEntry->second.stock;

	count += value;
}


//-----------------------------------------------------------------------------

void Inventory::addCoins(int n)
{
	coins_ += n;
}

bool Inventory::removeCoins(int n)
{
	if(coins_ - n < 0)
	{
		return false;
	}
	else
	{
		coins_ -= n;
		return true;
	}
}

int Inventory::getCoins() const
{
	return coins_;
}


//-----------------------------------------------------------------------------

std::map<PowerUpID::ID, sf::Keyboard::Key> Inventory::getKeys() const
{
	std::map<PowerUpID::ID, sf::Keyboard::Key> keys;
	for(auto cit=keyBindings_.cbegin(); cit!=keyBindings_.cend(); ++cit)
	{
		keys[cit->second] = cit->first;
	}
	return keys;
}
