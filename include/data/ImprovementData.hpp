#ifndef IMPROVEMENT_DATA_HPP
#define IMPROVEMENT_DATA_HPP


#include <vector>
#include <string>

#include "core/Improvement.hpp"

struct ImprovementData
{
	int baseValue;
	int increment;
	int basePrice;
	int priceIncrement;
	std::string description;
};

std::vector<ImprovementData> genImprovementData();


#endif // IMPROVEMENT_DATA_HPP
