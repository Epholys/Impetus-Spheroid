#ifndef DIFFICULTY_DATA_HPP
#define DIFFICULTY_DATA_HPP


#include <cmath>
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
	unsigned int probaEasy;
	unsigned int probaMedium;
	unsigned int probaHard;

	
};

struct DifficultyEventGenSeed
{
	int resolution;
	int attenuation;
	int offset;
};

std::vector<DifficultyWorld> genDifficultyWorld();
std::vector<DifficultyWorld> genDifficultyWorld(DifficultyWorldSeed seed);

std::vector<DifficultyEventGen> genDifficultyEvent();
std::vector<DifficultyEventGen> genDifficultyEvent(DifficultyEventGenSeed seed);


#endif // DIFFICULTY_DATA_HPP
