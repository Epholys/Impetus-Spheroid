#ifndef STATE_OVER_HPP
#define STATE_OVER_HPP


#include <array>
#include <algorithm>
#include <sstream>
#include <utility>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/State.hpp"
#include "framework/DataSaver.hpp"
#include "utility/utility.hpp"
#include "utility/Vector2.hpp"
#include "data/RankData.hpp"
#include "gui/Button.hpp"


// The Game Over screen State.

class StateOver : public State
{
public:
	enum Text
	{
		HighScore = 0,
		HighScoreRankText,
		HighScoreRank,
		ScoreText,
		ScoreNum,
		ScoreRankText,
		ScoreRank,
		MoneyWon,
		Money,
		GameOver,
		TextCount
	};

public:
	StateOver(StateStack& stack, Context context);
	virtual ~StateOver();

	virtual void draw(sf::RenderStates states);
	virtual bool update(Time dt);
	virtual bool handleInput(const sf::Event& event);
	
private:
	void initStaticTexts();
	void initVariableTexts(Context context);
	RankData getRank(int score);
	void initButtons(Vector2u winSize, const sf::Font& font);

	void updateDatas(Context context);

private:
	std::vector<sf::Text> texts_;
	std::vector<gui::Button> buttons_;
};


#endif // STATE_OVER_HPP
