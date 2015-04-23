#ifndef MARKET_DATA_HPP
#define MARKET_DATA_HPP


#include <vector>
#include <string>

#include "core/Identifiers.hpp"

struct MarketData
{
	PowerUpID::ID id;
	int number;
	int price;
	std::string description;
	std::string texturePath;
};


std::vector<MarketData> genMarketData();

#endif // MARKET_DATA_HPP
