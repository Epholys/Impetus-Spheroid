#include "data/TutorialData.hpp"


//-----------------------------------------------------------------------------

std::vector<TutorialData> genTutorialData()
{
	return
	{
		{"Welcome <testSubject>.", Vector2f(250, 280), false},
		{"Please shoot spheroids to this target.", Vector2f(65,280), true},
		{"You only have 20.00 human seconds per phase.", Vector2f(75,20), true},
		{"Here is your objective for the current phase.",Vector2f(75,210), true},
		{"Here is your current score for this phase.",Vector2f(100,510), true},
		{"If you exceed your objective,\nyou will be given humanReward: coins.", Vector2f(180, 50), true},
		{"I may or may not alter gravity, time,\nor other experiment variable. Please do your best\nto complete as many phases as possible.", Vector2f(100,200), false}
	};
	
}



