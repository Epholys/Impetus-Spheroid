#include "core/Inventory.hpp"


//-----------------------------------------------------------------------------

Inventory::Inventory(bool isAzerty)
	: keyBindings_(),
	  powerUps_(),
	  world_(nullptr)
{
	initKeyBinding(isAzerty);
}

Inventory::~Inventory()
{
}


//-----------------------------------------------------------------------------

void Inventory::initKeyBinding(bool isAzerty)
{
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
		auto gotTuple = powerUps_.find(id);
		if(gotTuple == powerUps_.end())
			return;

		auto tuple = gotTuple->second;
		auto& powerUp = std::get<0>(tuple);
		int& count = std::get<1>(tuple);
		if(count == 0)
			return;

		(*(powerUp))(*world_);

		if(powerUp->getType() == PowerUp::Modifier)
		{
			--count;
		}
	}
}

void Inventory::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
}


//-----------------------------------------------------------------------------

bool Inventory::decrement(PowerUpID::ID id)
{

	auto gotTuple = powerUps_.find(id);
	if(gotTuple == powerUps_.end())
		return false;

	auto tuple = gotTuple->second;
	auto& powerUp = std::get<0>(tuple);
	int& count = std::get<1>(tuple);
	// Absorb the error
	if(count == 0)
		return false;

	--count;

	if(powerUp->getType() == PowerUp::Toogle &&
	   count == 0 &&
	   world_)
	{
		// Launch the deactivate_() of powerUpToogle
		(*(powerUp))(*world_);
	}
	
	return true;
}

void Inventory::increment(PowerUpID::ID id, int value)
{
	auto gotTuple = powerUps_.find(id);
	if(gotTuple == powerUps_.end())
		return;

	auto tuple = gotTuple->second;
	int& count = std::get<1>(tuple);
	
	count += value;
}
