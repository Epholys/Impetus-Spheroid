#include "data/DifficultyData.hpp"


//-----------------------------------------------------------------------------

std::vector<DifficultyWorld> genDifficultyWorld()
{
	const float speedCoeff = 0.05f;

	return genDifficultyWorld(DifficultyWorldSeed{speedCoeff});
}

std::vector<DifficultyWorld> genDifficultyWorld(DifficultyWorldSeed seed)
{
	DifficultyWorld dw {seed.speedConstant};
	std::vector<DifficultyWorld> datas {dw};
	return datas;
}


//-----------------------------------------------------------------------------

std::vector<DifficultyEventGen> genDifficultyEvent()
{
	return std::vector<DifficultyEventGen>();
}
