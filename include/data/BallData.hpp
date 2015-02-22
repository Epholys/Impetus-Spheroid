#ifndef BALL_DATA_HPP
#define BALL_DATA_HPP


#include <vector>

#include <SFML/Graphics/Color.hpp>

struct BallData
{
	int point;
	
	// If proba==1 then there is 1/1000 to generate this ball.
	int proba;

	sf::Color color;
};

// The vector generataed will be read from below to top, so make sure that the
// first element have a proba of 1000.
std::vector<BallData> genBallDatas();


#endif // BALL_DATA_HPP
