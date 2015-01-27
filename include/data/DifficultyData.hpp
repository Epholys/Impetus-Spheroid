#ifndef DIFFICULTY_DATA_HPP
#define DIFFICULTY_DATA_HPP


#include <vector>

struct DifficultyWorld
{
	float speedConstant;
};

struct DifficultyEventGen
{
	
};

std::vector<DifficultyWorld> genDifficultyWorld();
std::vector<DifficultyEventGen> genDifficultyEvent();


#endif // DIFFICULTY_DATA_HPP
