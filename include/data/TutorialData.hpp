#ifndef TUTORIAL_DATA
#define TUTORIAL_DATA


#include <vector>
#include <string>

#include "utility/Vector2.hpp"

struct TutorialData
{
	std::string instruction;
	Vector2f position;
	bool hasArrow;
};

std::vector<TutorialData> genTutorialData();


#endif // TUTORIAL_DATA
