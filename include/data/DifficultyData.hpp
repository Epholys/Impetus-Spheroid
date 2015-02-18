#ifndef DIFFICULTY_DATA_HPP
#define DIFFICULTY_DATA_HPP


#include <vector>

struct DifficultyWorld
{
	float speedConstant;
};

struct DifficultyWorldSeed
{
	float speedConstant;
};

struct DifficultyEventGen
{
	
};

std::vector<DifficultyWorld> genDifficultyWorld();
std::vector<DifficultyWorld> genDifficultyWorld(DifficultyWorldSeed seed);

std::vector<DifficultyEventGen> genDifficultyEvent();


#endif // DIFFICULTY_DATA_HPP
