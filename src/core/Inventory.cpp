#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

Inventory::Inventory(bool isAzerty)
	: keyBindings_(),
	  keys_(),
	  powerUps_(),
	  inventory_(),
	  textures_(),
	  world_(nullptr)
{
	initKeyBinding(isAzerty);
	genPowerUps(powerUps_, inventory_, textures_);
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
}

Inventory::~Inventory()
{
}


//-----------------------------------------------------------------------------

void Inventory::initKeyBinding(bool isAzerty)
{
	using namespace PowerUpID;
	
	if(isAzerty)
	{
		keyBindings_[sf::Keyboard::A] = GhostBall;
	}
	else
	{
		keyBindings_[sf::Keyboard::Q] = GhostBall;
	}

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

	
	const Vector2u POWERUP_ICON_SIZE (20, 20);
	const int SPACE_BEETWEEN_ICONS = 10;
	const int POWERUP_PER_LINE = 5;
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

	int xCoord = targetSize.x / 2 - (POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS) * (POWERUP_PER_LINE) / 2;
	int yCoord = targetSize.y - (POWERUP_ICON_SIZE.y + SPACE_BEETWEEN_ICONS) * ((nPowerUp / POWERUP_PER_LINE) + 2);

	int nTextureDrawn=0;
	for(auto it = textures_.begin(); it != textures_.end(); ++it)
	{
		PowerUpID::ID id = it->first;

		sf::Sprite sprite;
		sprite.setTexture(it->second);
		sprite.setPosition(xCoord, yCoord);
		target.draw(sprite);

		sf::Text num;
		num.setFont(font_);
		num.setString(toString(inventory_.at(id)));
		num.setPosition(xCoord, yCoord + POWERUP_ICON_SIZE.y / 1.5);
		num.setCharacterSize(20);
		target.draw(num);

		sf::Text key;
		key.setFont(font_);
		key.setString(toString(keys_.at(id)));
		key.setPosition(xCoord, yCoord - POWERUP_ICON_SIZE.y);
		key.setCharacterSize(20);
		target.draw(key);

		++nTextureDrawn;
		
		int nextPositionInLine = nTextureDrawn % nPowerUp;
		if(nextPositionInLine != 0)
		{
			xCoord += POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS;
		}
		else
		{
			xCoord = targetSize.x / 2 - (POWERUP_ICON_SIZE.x + SPACE_BEETWEEN_ICONS) * (POWERUP_PER_LINE) / 2;
			yCoord += POWERUP_ICON_SIZE.y + SPACE_BEETWEEN_ICONS;
		}
	}
}


//-----------------------------------------------------------------------------

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
		// Launch the deactivate_() of powerUpToogle
		powerUp->apply(*world_);
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
