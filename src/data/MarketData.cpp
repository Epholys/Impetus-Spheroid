#include "data/MarketData.hpp"


//-----------------------------------------------------------------------------

std::vector<MarketData> genMarketData()
{
	using namespace PowerUpID;

	
	std::string folderPath = "./media/sprites/";
	std::string end = "button.png";
	
	std::vector<MarketData> datas
		{   {GhostBall, 10, 500,
		     "Ghost Balls which pass through",
		     folderPath + "GhostBall" + end},
		    
		    {NoGravBall, 10, 250,
		     "Massless Balls that ignore gravity", 
		     folderPath + "NoGravBall" + end},

		    {CancelEvents, 1, 1000,
		     "Powerful Event Canceler",
		     folderPath + "CancelEvents" + end},

		    {AddTime, 1, 2500,
		     "Add 5 seconds to the timer",
		     folderPath + "AddTime" + end},

		    {AddTarget, 1, 5000,
		     "Add a Target for 20s",
		     folderPath + "AddTarget" + end},
		    
		    {PointMultiplier, 1, 5000,
		     "Multiply the points by 1.5",
		     folderPath + "MultiplyPoints" + end},
		    
		    {BallTouchDouble, 10, 200,
		     "Balls that can touch twice",
		     folderPath + "BallTouchDouble" + end}   };

	return datas;
}
