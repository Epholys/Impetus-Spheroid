#ifndef STATE_OVER_HPP
#define STATE_OVER_HPP


#include <sstream>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "framework/State.hpp"
#include "framework/DataSaver.hpp"
#include "utility/utility.hpp"
#include "utility/Vector2.hpp"


// The Game Over screen State.

class StateOver : public State
{
public:
	enum Text
	{
		HighScore = 0,
		Score,
		Money,
		GameOver,
		Retry,
		Market,
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
	void initVariableTexts();
		void updateDatas();

private:
	sf::Font font_;
	std::vector<sf::Text> texts_;
};


#endif // STATE_OVER_HPP
