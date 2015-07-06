#ifndef META_DATA_HPP
#define META_DATA_HPP


#include <vector>

#include "core/Inventory.hpp"


/* All the datas that need to be persistent beetween several games and several
 * execution of the program.
 * */

struct MetaData
{
	int highScore;
	Inventory inventory;
	std::vector<bool> isPowerUpUnlocked;

	MetaData()
		: inventory(), isPowerUpUnlocked(PowerUpID::PowerUpCount, true)
		{
		}

	MetaData(
		Vector2u originalSize,
		bool isAzerty)
		: inventory(originalSize, isAzerty),
		  isPowerUpUnlocked(PowerUpID::PowerUpCount, true)
		{
		}
};


#endif
