#include "framework/StateOver.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto rankDatas = genRankData();
}


//-----------------------------------------------------------------------------

StateOver::StateOver(StateStack& stack, Context context)
	: State(stack, context)
	, texts_(TextCount)
{
	const sf::Font& font = context.fonts->get(FontID::ForcedSquare);
	for(int i=0; i<TextCount; i++)
	{
		texts_[i].setFont(font);
	}

	initStaticTexts();
	initVariableTexts(context);
	updateDatas(context);
}

StateOver::~StateOver()
{
}


//-----------------------------------------------------------------------------

RankData StateOver::getRank(int score)
{
	auto foundIt = std::find_if(rankDatas.rbegin(),
	                            rankDatas.rend(),
	                            [score](const RankData& data)
	                            {
		                            return data.lowerBound <= score;
	                            });
	if(foundIt == rankDatas.rend())
	{
		assert(!rankDatas.empty());
		--foundIt;
	}

	return *foundIt;
}


//-----------------------------------------------------------------------------

void StateOver::initStaticTexts()
{
	Vector2u winSize = context_.originalWindowSize;
	const Vector2f GAME_OVER_POS (winSize.x / 2, winSize.y / 2);
	const Vector2f MARKET_POS (winSize.x / 2, 2 * winSize.y / 3);
	const Vector2f RETRY_POS (winSize.x / 2, 4 * winSize.y / 5);
	const sf::Color DEFAULT (255,255,255);
	const sf::Color HIGHLIGHT (0, 200, 255);
	bool centerOrigin = true;
	
	defineText(texts_[GameOver], "G A M E  O V E R", GAME_OVER_POS);

	
	defineText(texts_[MarketPre], "Press ", Vector2f(), DEFAULT, !centerOrigin);
	defineText(texts_[MarketKey], "CTRL ", Vector2f(texts_[MarketPre].findCharacterPos(100)), HIGHLIGHT, !centerOrigin);
	defineText(texts_[MarketPost], "to go to the Market", Vector2f(texts_[MarketKey].findCharacterPos(100)), DEFAULT, !centerOrigin);
	moveGroupOfTexts(MARKET_POS, texts_[MarketPre], texts_[MarketPost], texts_[MarketKey]);

   
	defineText(texts_[RetryPre], "Press ", Vector2f(), DEFAULT, !centerOrigin);
	defineText(texts_[RetryKey], "SPACE ", Vector2f(texts_[RetryPre].findCharacterPos(100)), HIGHLIGHT, !centerOrigin);
	defineText(texts_[RetryPost], "to retry", Vector2f(texts_[RetryKey].findCharacterPos(100)), DEFAULT, !centerOrigin);
	moveGroupOfTexts(RETRY_POS, texts_[RetryPre], texts_[RetryPost], texts_[RetryKey]);
}

void StateOver::initVariableTexts(Context context)
{
	Vector2u winSize = context_.originalWindowSize;
	const MetaData* const metaData = context_.metaData;
	int lastObjective = context_.lastGameData->lastObjective;

	const Vector2f HIGH_SCORE_POS (winSize.x / 2, 2*winSize.y / 20);
	const Vector2f HIGH_SCORE_RANK_POS (winSize.x / 2, 2.5*winSize.y / 20); 
	const Vector2f SCORE_POS (winSize.x / 4, 5*winSize.y / 20);
	const Vector2f SCORE_RANK_POS (winSize.x / 4, 6*winSize.y/ 20);
	const Vector2f MONEY_POS (3* winSize.x / 5, 5*winSize.y / 20);

	int coinsWon = std::max(0, lastObjective - DifficultyManager::BASE_OBJECTIVE_)
		           * context.metaData->improvementValue[ImprovementID::CoinsPerPointObjective];

	const sf::Color DEFAULT (255,255,255);
	bool centerOrigin = true;
	
	std::stringstream ss;

	int highScore = std::max(metaData->highScore, lastObjective); 
	ss << "High Score: "<< highScore;
	defineText(texts_[HighScore], ss.str(), HIGH_SCORE_POS);
	ss.str("");

	RankData highScoreRank = getRank(highScore);
	defineText(texts_[HighScoreRankText], "Rank: ", Vector2f(), DEFAULT, !centerOrigin);
	ss << highScoreRank.rank;
	defineText(texts_[HighScoreRank], ss.str(), Vector2f(texts_[HighScoreRankText].findCharacterPos(100)), highScoreRank.color, !centerOrigin);
	ss.str("");
	moveGroupOfTexts(HIGH_SCORE_RANK_POS, texts_[HighScoreRankText], texts_[HighScoreRank]);

	defineText(texts_[ScoreText], "Your score: ", Vector2f(), DEFAULT, !centerOrigin);
	ss << lastObjective<< "  ";
	defineText(texts_[ScoreNum], ss.str(), Vector2f(texts_[ScoreText].findCharacterPos(100)), sf::Color::Green, !centerOrigin);
	ss.str("");
	ss << "+ "  << coinsWon;
	defineText(texts_[MoneyWon], ss.str(), Vector2f(texts_[ScoreNum].findCharacterPos(100)), sf::Color::Yellow, !centerOrigin);
	ss.str("");
	moveGroupOfTexts(SCORE_POS, texts_[ScoreText], texts_[MoneyWon], texts_[ScoreNum]);
	
	RankData scoreRank = getRank(lastObjective);
	defineText(texts_[ScoreRankText], "Rank: ", Vector2f(), DEFAULT, !centerOrigin);
	ss << scoreRank.rank;
	defineText(texts_[ScoreRank], ss.str(), Vector2f(texts_[ScoreRankText].findCharacterPos(100)), scoreRank.color, !centerOrigin);
	ss.str("");
	moveGroupOfTexts(SCORE_RANK_POS, texts_[ScoreRankText], texts_[ScoreRank]);
		
		
	ss <<  "C O I N S : ";
	ss << metaData->inventory.getCoins() + coinsWon;
	defineText(texts_[Money], ss.str(), MONEY_POS, sf::Color::Yellow, !centerOrigin);
}

//-----------------------------------------------------------------------------

void StateOver::updateDatas(Context context)
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
		* context.metaData->improvementValue[ImprovementID::CoinsPerPointObjective]);

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

