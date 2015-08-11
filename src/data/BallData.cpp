#include "data/BallData.hpp"


//-----------------------------------------------------------------------------

std::vector<BallData> genBallDatas()
{
	std::vector<BallData> datas 
	{   {1,   1000, sf::Color::Red},
		{2,   100,  sf::Color(255,130,0)},
		{5,   25,   sf::Color(255,230,0)},
		{10,  10,   sf::Color::Green},
		{20,  5,    sf::Color(0,255,255)},
		{50,  2,    sf::Color::Blue},
		{100, 1,    sf::Color(125,0,255)} };

	return datas;		
}
