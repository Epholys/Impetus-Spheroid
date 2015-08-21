#include "data/TutorialData.hpp"


//-----------------------------------------------------------------------------

std::vector<TutorialData> genTutorialData()
{
	return
	{
		{"Welcome <testSubject>.", Vector2f(250, 280), false},
		{"Please shoot spheroids to this target.", Vector2f(65,280), true},
		{"You only have 20.00 human seconds per phase.", Vector2f(75,0), true},
		{"Points/Objective for the current phase.",Vector2f(150,20), true},
		{"If you exceed your objective,\nyou will be given humanReward: coins.", Vector2f(180,20), true},
		{"I may or may not alter gravity, time or other experiment variable.\nPlease do your best to complete as many phases as possible.", Vector2f(27,200), false}
	};
	
}
