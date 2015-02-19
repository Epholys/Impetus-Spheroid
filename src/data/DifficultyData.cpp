#include "data/DifficultyData.hpp"


//-----------------------------------------------------------------------------

std::vector<DifficultyWorld> genDifficultyWorld()
{
	const float speedCoeff = 0.05f;

	return genDifficultyWorld(DifficultyWorldSeed{speedCoeff});
}

std::vector<DifficultyWorld> genDifficultyWorld(DifficultyWorldSeed seed)
{
	DifficultyWorld dw {seed.speedConstant};
	std::vector<DifficultyWorld> datas {dw};
	return datas;
}


//-----------------------------------------------------------------------------

std::vector<DifficultyEventGen> genDifficultyEvent()
{
	const unsigned int RESOLUTION = 100;
	const unsigned int ATTENUATION = 8;
	const unsigned int OFFSET = 10;

	const DifficultyEventGenSeed seed {RESOLUTION, ATTENUATION, OFFSET};

	return genDifficultyEvent(seed);
}


// Please paste this into wolfram alpha:
// Plot[{(66 (Pi/2 - ArcTan[(-10 + x)/8]))/Pi, (66 (Pi/2 + ArcTan[(-10 + x)/8]))/Pi, x}, {x, 0, 30}]

std::vector<DifficultyEventGen> genDifficultyEvent(DifficultyEventGenSeed seed)
{
	const float PI = 3.14159f;
	const int PHASE_MAX = 50;
	seed.attenuation = (seed.attenuation == 0) ? 1 : seed.attenuation;
	
	std::vector<DifficultyEventGen> vdeg;

	for (int i=0; i<PHASE_MAX; ++i)
	{
		DifficultyEventGen deg;
		deg.probaEasy = (seed.resolution/PI) * ((PI/2) - std::atan((i - seed.offset)/seed.attenuation));
		deg.probaHard = (seed.resolution/PI) * ((PI/2) + std::atan((i - seed.offset)/seed.attenuation));
		deg.probaMedium = (deg.probaEasy + deg.probaHard) / 2;
		vdeg.push_back(deg);
	}

	return vdeg;
}
