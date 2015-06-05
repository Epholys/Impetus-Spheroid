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

	initStaticTexts(context);
	initVariableTexts(context);
	updateDatas(context);
}

StateOver::~StateOver()
{
}


//-----------------------------------------------------------------------------

void StateOver::initStaticTexts(Context context)
{
	Vector2u winSize = Vector2u(context.originalWindowSize);
	const Vector2f GAME_OVER_POS (winSize.x / 2, winSize.y / 2);
	const Vector2f MARKET_POS (winSize.x / 2, 2 * winSize.y / 3);
	const Vector2f RETRY_POS (winSize.x / 2, 4 * winSize.y / 5); 

	texts_[GameOver].setString("G A M E  O V E R");
	centerOrigin(texts_[GameOver]);
	texts_[GameOver].setPosition(GAME_OVER_POS);

	texts_[Market].setString("Press CTRL to go to the Market");
	centerOrigin(texts_[Market]);
	texts_[Market].setPosition(MARKET_POS);

	texts_[Retry].setString("Press SPACE to retry");
	centerOrigin(texts_[Retry]);
	texts_[Retry].setPosition(RETRY_POS);
}

void StateOver::initVariableTexts(Context context)
{
	Vector2u winSize = Vector2u(context.originalWindowSize );
	
	std::stringstream ss;

	const Vector2f HIGH_SCORE_POS (winSize.x / 2, winSize.y / 10);
	ss << "High Score: ";
	ss << std::max(context_.datas->highScore, context_.datas->lastHighScore);
	texts_[HighScore].setString(ss.str());
	centerOrigin(texts_[HighScore]);
	texts_[HighScore].setPosition(HIGH_SCORE_POS);
	ss.str("");

	const Vector2f SCORE_POS (winSize.x / 4, winSize.y / 5);
	ss << "Your Score: ";
	ss << context_.datas->lastHighScore;
	ss << ": +";
	ss << std::max(0, context_.datas->lastHighScore - context_.datas->BASE_OBJECTIVE) * context.datas->COINS_PER_POINTS;
	ss << " coins";
	texts_[Score].setString(ss.str());
	centerOrigin(texts_[Score]);
	texts_[Score].setPosition(SCORE_POS);
	ss.str("");	

	const Vector2f MONEY_POS (3* winSize.x / 4, winSize.y / 5);
	ss <<  "C O I N S : ";
	ss << context_.datas->inventory.getCoins() + std::max(0, context_.datas->lastHighScore - context_.datas->BASE_OBJECTIVE) * context.datas->COINS_PER_POINTS;
	texts_[Money].setString(ss.str());
	centerOrigin(texts_[Money]);
	texts_[Money].setPosition(MONEY_POS);
	texts_[Money].setColor(sf::Color::Yellow);
	ss.str("");	
}


//-----------------------------------------------------------------------------

void StateOver::updateDatas(Context context)
{
	int newScore = context_.datas->lastHighScore;
	if(newScore > context_.datas->highScore)
	{
		texts_[HighScore].setColor(sf::Color::Green);
		context_.datas->highScore = newScore;
	}
	else
	{
		texts_[HighScore].setColor(sf::Color::Red);
	}
	context_.datas->inventory.addCoins(std::max(0, context_.datas->lastHighScore - context_.datas->BASE_OBJECTIVE) * context.datas->COINS_PER_POINTS);

	DataSaver::saveDatas(*context_.datas);
}


//-----------------------------------------------------------------------------

void StateOver::draw()
{
	sf::RectangleShape rect (Vector2f(context_.window->getSize()));
	rect.setFillColor(sf::Color(0,0,0,200));
	context_.window->draw(rect);
	sf::RenderStates states;
	states.transform *= *context_.globalTransform;
	for(int i=0; i<TextCount; ++i)
	{
		context_.window->draw(texts_[i], states);
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
		requestStackPop();
		requestStackPop();
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

