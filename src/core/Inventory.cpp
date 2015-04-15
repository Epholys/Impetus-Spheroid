#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

Inventory::Inventory(bool isAzerty)
	: font_()
	, coins_(0)
	, keyBindings_()
	, keys_()
	, powerUps_()
	, inventory_()
	, textures_()
	, world_(nullptr)
{
	initKeyBinding(isAzerty);
	genPowerUps(powerUps_, inventory_, textures_);
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	const Vector2f COINS_POSITION (710.f, 40.f);
	coinsText_.setFont(font_);
	coinsText_.setPosition(COINS_POSITION);
	coinsText_.setString(toString(coins_));
	coinsText_.setColor(sf::Color::Yellow);
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
		keyBindings_[sf::Keyboard::Q] = AutoFire;
		keyBindings_[sf::Keyboard::W] = PointMultiplier;
	}
	else
	{
		keyBindings_[sf::Keyboard::Q] = GhostBall;
		keyBindings_[sf::Keyboard::W] = NoGravBall;
		keyBindings_[sf::Keyboard::A] = AutoFire;
		keyBindings_[sf::Keyboard::Z] = PointMultiplier;
	}

	keyBindings_[sf::Keyboard::E] = CancelEvents;
	keyBindings_[sf::Keyboard::S] = AddTime;
	keyBindings_[sf::Keyboard::D] = AddTarget;
	keyBindings_[sf::Keyboard::X] = BallTouchDouble;

	for(auto it = keyBindings_.begin(); it != keyBindings_.end(); ++it) 
	{
		keys_[it->second] = it->first;
	}
}

void Inventory::addWorld(World* world)
{
	world_ = world;
}

void Inventory::removeWorld()
{
	world_ = nullptr;
}


//-----------------------------------------------------------------------------

void Inventory::handleInput(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		// TODO Modify later for the market
		if(!world_)
			return;

		auto gotId = keyBindings_.find(event.key.code);
		if(gotId == keyBindings_.end())
			return;
		
		auto id = gotId->second;
		auto gotPowerUp = powerUps_.find(id);
		if(gotPowerUp == powerUps_.end())
			return;

		auto& powerUp = gotPowerUp->second;
		int& count = inventory_[id];
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
	
	states.transform *= getTransform();


	const std::string str = toString(coins_);
	coinsText_.setString(str);
	target.draw(coinsText_);

	
	const Vector2u POWERUP_ICON_SIZE (20, 20);
	const Vector2u SPACE_BEETWEEN_ICONS (10, 30);
	const int POWERUP_PER_LINE = 10;
	auto targetSize = target.getSize();

	int nPowerUp = std::count_if(inventory_.begin(),
	                             inventory_.end(),
//	                             [](const auto& pair)
	                             [](const std::pair<PowerUpID::ID, int> pair)
	                             {
		                             return pair.second > 0;
	                             });

	if(nPowerUp == 0)
		return;

	int xCoord = targetSize.x / 2 - (POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS.x) * (POWERUP_PER_LINE) / 2;
	int yCoord = targetSize.y * 0.90f - (POWERUP_ICON_SIZE.y + SPACE_BEETWEEN_ICONS.y) * ((nPowerUp-1) / POWERUP_PER_LINE);

	int nTextureDrawn=0;
	for(auto it = textures_.begin(); it != textures_.end(); ++it)
	{
		PowerUpID::ID id = it->first;

		if(inventory_.at(id)>0) 
		{
			sf::Text num;
			num.setFont(font_);
			num.setString(toString(inventory_.at(id)));
			num.setPosition(xCoord, yCoord + POWERUP_ICON_SIZE.y / 1.5);
			num.setCharacterSize(20);

			sf::Sprite sprite;
			sprite.setTexture(it->second);
			sprite.setPosition(xCoord, yCoord);

			sf::Text key;
			key.setFont(font_);
			key.setString(toString(keys_.at(id)));
			key.setPosition(xCoord, yCoord - POWERUP_ICON_SIZE.y);
			key.setCharacterSize(20);

			target.draw(key);
			target.draw(sprite);
			target.draw(num);

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

	auto gotCount = inventory_.find(id);
	if(gotCount == inventory_.end())
		return false;

	int& count = inventory_[id];
	count = value;

	return true;
}

int Inventory::getPowerUp(PowerUpID::ID id) const
{
	auto gotCount = inventory_.find(id);
	if(gotCount == inventory_.end())
		return -1;

	return inventory_.at(id);
}


bool Inventory::decrement(PowerUpID::ID id)
{

	auto gotPowerUp = powerUps_.find(id);
	if(gotPowerUp == powerUps_.end())
		return false;

	auto& powerUp = gotPowerUp->second;
	int& count = inventory_[id];
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
	auto gotNumber = inventory_.find(id);
	if(gotNumber == inventory_.end())
		return;

	int& count = gotNumber->second;
	
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
