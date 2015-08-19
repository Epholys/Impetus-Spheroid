#ifndef META_DATA_HPP
#define META_DATA_HPP


#include <vector>

#include "data/ImprovementData.hpp"
#include "core/Inventory.hpp"


/* All the datas that need to be persistent beetween several games and several
 * execution of the program.
 * */

struct MetaData
{
	bool firstTimeExecuted;
	int highScore;
	Inventory inventory;
	std::vector<bool> isPowerUpUnlocked;
	std::vector<int> improvementValue;

	MetaData()
		: MetaData({800u,600u}, true)
		{
		}

	MetaData(
		Vector2u originalSize,
		bool isAzerty)
		: inventory(originalSize, isAzerty),
		  isPowerUpUnlocked(PowerUpID::PowerUpCount, true),
		  improvementValue(ImprovementID::ImprovementCount, 1)
		{
		}
};


#endif
