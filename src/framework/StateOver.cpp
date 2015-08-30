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
	, buttons_(2, gui::Button())
{
	const sf::Font& font = context.fonts->get(FontID::ForcedSquare);
	for(int i=0; i<TextCount; i++)
	{
		texts_[i].setFont(font);
	}

	initStaticTexts();
	initVariableTexts(context);
	initButtons(context.originalWindowSize, font);
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
	
	defineText(texts_[GameOver], "G A M E  O V E R", GAME_OVER_POS);
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

void StateOver::initButtons(Vector2u winSize, const sf::Font& font)
{
	const Vector2f TEXTURE_SIZE {650.f, 50.f};
	const Vector2f MARKET_POS ((winSize.x - TEXTURE_SIZE.x) / 2, 2 * winSize.y / 3);
	const Vector2f RETRY_POS ((winSize.x - TEXTURE_SIZE.x) / 2, 4 * winSize.y / 5);

	buttons_[0].setCallback(
		[this]()
		{ requestStackPop();
		  requestStackPush(StateID::Market); });
	buttons_[0].setDefaultTexture();
	buttons_[0].setAlpha(50);
	buttons_[0].setLabel(gui::Button::Middle, "Market", font);
	buttons_[0].setKey(sf::Keyboard::LControl);
	buttons_[0].move(MARKET_POS);

	
	buttons_[1].setCallback(
		[this]()
		{ requestStackClear();
		  requestStackPush(StateID::Game); });
	buttons_[1].setDefaultTexture();
	buttons_[1].setAlpha(50);
	buttons_[1].setLabel(gui::Button::Middle, "Retry", font);
	buttons_[1].setKey(sf::Keyboard::Space);
	buttons_[1].move(RETRY_POS);		
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

	for(const auto& button : buttons_)
	{
		window->draw(button, states);
	}
}

bool StateOver::update(Time)
{
	return true;
}

bool StateOver::handleInput(const sf::Event& event)
{
	for(auto& button : buttons_)
	{
		button.handleEvent(event);
	}
	
	return false;
}

