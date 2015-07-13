#ifndef RANK_DATA_HPP
#define RANK_DATA_HPP


#include <vector>

#include <SFML/Graphics/Color.hpp>

struct RankData
{
	char rank;
	int lowerBound;
	sf::Color color;
};

std::vector<RankData> genRankData();


#endif // RANK_DATA_HPP
