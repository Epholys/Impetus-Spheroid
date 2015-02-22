#include "data/BallData.hpp"


//-----------------------------------------------------------------------------

std::vector<BallData> genBallDatas()
{
	std::vector<BallData> datas 
	{   {1,   1000, sf::Color::Red},
		{2,   100,  sf::Color(255,125,0)},
		{5,   25,   sf::Color::Yellow},
		{10,  10,   sf::Color(125,255,0)},
	 	{20,  5,    sf::Color::Green},
		{50,  2,    sf::Color(0,255,125)},
		{100, 1,    sf::Color::Blue} };

	return datas;		
}
