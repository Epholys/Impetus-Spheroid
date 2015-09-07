#include "data/TutorialData.hpp"


//-----------------------------------------------------------------------------

std::vector<TutorialData> genTutorialData()
{
	bool hasArrow = true;
	return
	{
		{"Welcome <testSubject>.", Vector2f(250, 280), !hasArrow},
		{"Please shoot spheroids at this target.", Vector2f(65,280), hasArrow},
		{"You only have 20.00 human seconds per phase.", Vector2f(75,20), hasArrow},
		{"Here is your objective for the current phase.",Vector2f(75,210), hasArrow},
		// {"Here is your current score for this phase.",Vector2f(100,510), hasArrow},
		{"If you exceed your objective,\nyou will be given humanReward: coins.", Vector2f(180, 50), hasArrow},
		{"I may or may not alter gravity, time,\nor other experiment variables."/* Please do your best\nto complete as many phases as possible."*/, Vector2f(200,200), !hasArrow}
	};
	
}
