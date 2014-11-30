#include "utility/random.hpp"


namespace
{
	std::default_random_engine createRandomEngine()
	{
		auto seed = static_cast<unsigned long>(std::time(nullptr));
		return std::default_random_engine(seed);
	}

	auto randomEngine = createRandomEngine();
}

int randInt(int minInclusive, int maxInclusive)
{
	std::uniform_int_distribution<> dist (minInclusive, maxInclusive);

	return dist(randomEngine);
}

float normalRandFloat(float mean, float stddev)
{
	std::normal_distribution<float> dist (mean, stddev);

	return dist(randomEngine);
}

