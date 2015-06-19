#include "framework/StateOver.hpp"


//-----------------------------------------------------------------------------

StateOver::StateOver(StateStack& stack, Context context)
	: State(stack, context)
	, font_()
	, texts_(TextCount)
{
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");
	for(int i=0; i<TextCount; i++)
	{
		texts_[i].setFont(font_);
	}

	initStaticTexts();
	initVariableTexts();
	updateDatas();
}

StateOver::~StateOver()
{
}


//-----------------------------------------------------------------------------

void StateOver::initStaticTexts()
{
	Vector2u winSize = context_.originalWindowSize;
	const Vector2f GAME_OVER_POS (winSize.x / 2, winSize.y / 2);
	const Vector2f MARKET_POS (winSize.x / 2, 2 * winSize.y / 3);
	const Vector2f RETRY_POS (winSize.x / 2, 4 * winSize.y / 5); 

	defineText(texts_[GameOver], "G A M E  O V E R", GAME_OVER_POS);
	defineText(texts_[Market], "Press CTRL to go to the Market", MARKET_POS);
	defineText(texts_[Retry], "Press SPACE to retry", RETRY_POS);
}

void StateOver::initVariableTexts()
{
	Vector2u winSize = context_.originalWindowSize;
	const MetaData* const metaData = context_.metaData;
	int lastObjective = context_.lastGameData->lastObjective;

	const Vector2f HIGH_SCORE_POS (winSize.x / 2, winSize.y / 10);
	const Vector2f SCORE_POS (winSize.x / 4, winSize.y / 5);
	const Vector2f MONEY_POS (3* winSize.x / 4, winSize.y / 5);

	int coinsWon = std::max(0, lastObjective - DifficultyManager::BASE_OBJECTIVE_)
		                    * DifficultyManager::COINS_PER_POINTS_;

	std::stringstream ss;

	ss << "High Score: ";
	ss << std::max(metaData->highScore, lastObjective);
	defineText(texts_[HighScore], ss.str(), HIGH_SCORE_POS);
	ss.str("");

	ss << "Your Score: ";
	ss << lastObjective;
	ss << ": +";
	ss << coinsWon;
	ss << " coins";
	defineText(texts_[Score], ss.str(), SCORE_POS);
	ss.str("");	

	ss <<  "C O I N S : ";
	ss << metaData->inventory.getCoins() + coinsWon;
	defineText(texts_[Money], ss.str(), MONEY_POS, sf::Color::Yellow);
}

//-----------------------------------------------------------------------------

void StateOver::updateDatas()
{
	MetaData* const metaData = context_.metaData;
	int lastObjective = context_.lastGameData->lastObjective;

	int newScore = lastObjective;
	if(newScore > metaData->highScore)
	{
		texts_[HighScore].setColor(sf::Color::Green);
		metaData->highScore = newScore;
	}
	else
	{
		texts_[HighScore].setColor(sf::Color::Red);
	}
	metaData->inventory.addCoins(
		std::max(0, lastObjective - DifficultyManager::BASE_OBJECTIVE_)
		* DifficultyManager::COINS_PER_POINTS_);

	DataSaver::saveDatas(*metaData);
}


//-----------------------------------------------------------------------------

void StateOver::draw(sf::RenderStates states)
{
	sf::RenderWindow* window = context_.window;

	sf::RectangleShape rect (Vector2f(window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	window->draw(rect);

	for(int i=0; i<TextCount; ++i)
	{
		window->draw(texts_[i], states);
	}
}

bool StateOver::update(Time)
{
	return true;
}

bool StateOver::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased &&
	   event.key.code == sf::Keyboard::Space)
	{
		requestStackClear();
		requestStackPush(StateID::Game);
	}

	if(event.type == sf::Event::KeyReleased &&
	   (event.key.code == sf::Keyboard::LControl ||
		event.key.code == sf::Keyboard::RControl))
	{ 
		requestStackPop();
		requestStackPush(StateID::Market);
	}

	return false;
}

