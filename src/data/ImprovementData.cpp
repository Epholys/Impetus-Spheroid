#include "data/ImprovementData.hpp"

//-----------------------------------------------------------------------------

std::vector<ImprovementData> genImprovementData()
{
	using namespace ImprovementID;
	
	std::vector<ImprovementData> datas (ImprovementCount);

	datas[BallsPerSecond].baseValue = 4;
	datas[BallsPerSecond].increment = 1;
	datas[BallsPerSecond].basePrice = 500;
	datas[BallsPerSecond].priceIncrement = 500;
	datas[BallsPerSecond].description = "balls per second";

	datas[CoinsPerPointScore].baseValue = 1;
	datas[CoinsPerPointScore].increment = 1;
	datas[CoinsPerPointScore].basePrice = 750;
	datas[CoinsPerPointScore].priceIncrement = 250;
	datas[CoinsPerPointScore].description = "coins per point (score)";

	datas[CoinsPerPointObjective].baseValue = 10;
	datas[CoinsPerPointObjective].increment = 2;
	datas[CoinsPerPointObjective].basePrice = 750;
	datas[CoinsPerPointObjective].priceIncrement = 250;
	datas[CoinsPerPointObjective].description = "coins per point (objective)";

	return datas;
}
