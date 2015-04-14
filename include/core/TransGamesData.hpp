#ifndef TRANS_GAMES_DATA_HPP
#define TRANS_GAMES_DATA_HPP


#include "core/Inventory.hpp"

struct TransGamesData
{
	int highScore;
	int lastHighScore;
	Inventory inventory;

	static const int COINS_PER_POINTS = 10;
	static const int BASE_OBJECTIVE = 20;
};


#endif
