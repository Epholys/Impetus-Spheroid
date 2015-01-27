#include "data/DifficultyData.hpp"


//-----------------------------------------------------------------------------

std::vector<DifficultyWorld> genDifficultyWorld()
{
	DifficultyWorld dw {0.05f};
	std::vector<DifficultyWorld> datas {dw};
	return datas;
}


//-----------------------------------------------------------------------------

std::vector<DifficultyEventGen> genDifficultyEvent()
{
	return std::vector<DifficultyEventGen>();
}
