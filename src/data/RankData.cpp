#include "data/RankData.hpp"


//-----------------------------------------------------------------------------

std::vector<RankData> genRankData()
{
	std::vector<RankData> datas =
		{
			{'F', 20,  sf::Color::Red},
			{'E', 30,  sf::Color(255,175,0)},
			{'D', 50,  sf::Color::Yellow},
			{'C', 70,  sf::Color::Green},
			{'B', 100, sf::Color(0,255,255)},
			{'A', 150, sf::Color(0, 75, 255)},
			{'S', 200, sf::Color(150,0,255)}
		};
	return datas;
}
