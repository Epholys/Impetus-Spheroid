#include "data/MarketData.hpp"


//-----------------------------------------------------------------------------

std::vector<MarketData> genMarketData()
{
	using namespace PowerUpID;

	
	std::string folderPath = "./media/sprites/";
	std::string end = "button.png";
	// std::string end = ".png";
	
	std::vector<MarketData> datas
		{   {GhostBall, 10, 250,
		     "Ghost Balls which pass through",
		     folderPath + "GhostBall" + end},
		    
		    {NoGravBall, 10, 200,
		     "Massless Balls that ignore gravity", 
		     folderPath + "NoGravBall" + end},

		    {BallTouchDouble, 10, 500,
			 "Balls that can touch twice",
			 folderPath + "BallTouchDouble" + end},
		    


		    {CancelEvents, 1, 750,
		     "Powerful Event Canceler",
		     folderPath + "CancelEvents" + end},

		    {AddTime, 1, 1000,
		     "Add 5 seconds to the timer",
		     folderPath + "AddTime" + end},

		    {AddTarget, 1, 2000,
		     "Add a Target for 20s",
		     folderPath + "AddTarget" + end},
		    
		    {PointMultiplier, 1, 2000,
		     "Multiply the points by 1.5",
		     folderPath + "MultiplyPoints" + end},
		    
		    {AddCannon, 1, 2500,
			 "Add a Cannon for 20s",
			 folderPath + "AddCannon" + end}
		};

	return datas;
}
