#ifndef META_DATA_HPP
#define META_DATA_HPP


#include "core/Inventory.hpp"

struct MetaData
{
	int highScore;
	int lastScore;
	Inventory inventory;

	static const int COINS_PER_POINTS = 10;
	static const int BASE_OBJECTIVE = 20;

	MetaData()
		: inventory()
		{
		}

	MetaData(
		Vector2u originalSize,
		bool isAzerty)
		: inventory(originalSize, isAzerty)
		{
		}
};


#endif
