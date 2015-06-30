#include <sstream>

#include "framework/Assertion.hpp"
#include "core/DifficultyManager.hpp"
#include "core/World.hpp"
#include "core/EventGenerator.hpp"
#include "core/MetaData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto worldDatas = genDifficultyWorld();
	auto eventDatas = genDifficultyEvent();
	auto ballDatas = genBallDatas();
}


//-----------------------------------------------------------------------------
// *** public static const attribute: ***

const int DifficultyManager::COINS_PER_POINTS_ = 10;
const int DifficultyManager::BASE_OBJECTIVE_ = 20;
const Time DifficultyManager::PHASE_TIME_ {seconds(20.f)};


//-----------------------------------------------------------------------------
// *** other constants: ***
namespace
{
	const int BASE_CEILING = 20;
	const int OBJECTIVE_INCREMENT = 2;

	const Vector2f INDICATOR_POSITION (720.f, 65.f);
	const Vector2f INDICATOR_SPACE (0.f, 15.f);
	const Time INDICATOR_DURATION = seconds(1.f);
	const Time DEQUE_DURATION = milliseconds(200);
}


//-----------------------------------------------------------------------------

DifficultyManager::DifficultyManager(DifficultyContext context)
	: phaseTime_()
	, phaseNumber_(0)
	, context_(context)
	, font_()
	, timer_()
	, score_(0.f)
	, objective_(BASE_OBJECTIVE_)
	, ceiling_(BASE_CEILING)
	, scoreText_()
	, indicatorTexts_()
	, indicatorDeque_(INDICATOR_POSITION, INDICATOR_SPACE, gui::Transition::Linear, DEQUE_DURATION)
	, indicatorAccumulator_()
	, indicatorDuration_(INDICATOR_DURATION)
	// , ballCount_()
	// , diffGui_(nullptr)
	// , maskGui_(true)
{
	const Vector2u WINDOW_SIZE = context_.world->getWindowSize();
	const Vector2f TIMER_POSITION (WINDOW_SIZE.x - 90.f, 0.f);
	const Vector2f SCORE_POSITION (WINDOW_SIZE.x - 90.f, 20.f);
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	timer_.setFont(font_);
	timer_.setPosition(TIMER_POSITION);
	timer_.setString("0:00");
	timer_.setColor(sf::Color::Black);

	scoreText_.setFont(font_);
	scoreText_.setPosition(SCORE_POSITION);
	updateScore();
	
//	createGui();

	context_.eventGenerator->updateDifficulty(eventDatas[0]);
}

// Served for quick parameter modification for prototyping, but the game's core
// is well implemented, so it isn't useful for now.
// void DifficultyManager::createGui()
// {
// }

DifficultyManager::~DifficultyManager()
{
}


//-----------------------------------------------------------------------------

void DifficultyManager::update(Time dt)
{
	phaseTime_ += dt;

	updateIndicatorDuration();
	indicatorDeque_.update(dt);
	if(!indicatorTexts_.empty())
	{
		indicatorAccumulator_ += dt;
	}
	if(indicatorAccumulator_ > indicatorDuration_)
	{
		indicatorAccumulator_ = Time();
		if(!indicatorTexts_.empty())
		{
			indicatorDeque_.popFront();
			indicatorTexts_.pop_front();
		}
	}	

	updateScore();

	if (phaseTime_ >= PHASE_TIME_)
	{
		phaseTime_ -= PHASE_TIME_;
		++phaseNumber_;
		updateDifficulty();
	}

	Time timerRemaining = PHASE_TIME_ - phaseTime_;
	std::stringstream ssSeconds;
	ssSeconds << int(timerRemaining.asSeconds());
	std::stringstream ssCentiSecs;
	ssCentiSecs << (timerRemaining.asMilliseconds() / 10) % 100;
	timer_.setString(ssSeconds.str() + ":" + ssCentiSecs.str());
}

// Used to forward instruction to the GUI 
// void DifficultyManager::handleInput(const sf::Event&)
// {
// }


void DifficultyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(timer_, states);
	target.draw(scoreText_, states);

	for (const auto& text : indicatorTexts_)
	{
		target.draw(text, states);
	}

	// Don't forget to draw the GUI if necessary!
}

//-----------------------------------------------------------------------------


void DifficultyManager::addTime(Time adding)
{
	phaseTime_ -= adding;
}

int DifficultyManager::getObjective() const
{
	return objective_;
}


//-----------------------------------------------------------------------------

void DifficultyManager::updateDifficulty()
{
	updateObjective();

	auto worldIndex = selectDataIndex(worldDatas);
	context_.world->updateDifficulty(worldDatas[worldIndex]);

	auto egenIndex = selectDataIndex(eventDatas);
	context_.eventGenerator->updateDifficulty(eventDatas[egenIndex]);
}

void DifficultyManager::updateScore()
{
	int COINS_PER_EXCESS = 1;

	auto collisions = context_.world->getTrackedCollisions();
	auto ecs = context_.world->getEntityManager();
	float points = 0;
	for(const auto& pair : collisions)
	{
		auto projectileComp =
			dynCast<ecs::Projectile>(ecs.getComponent(pair.first,
			                                          ecs::Component::Projectile));
		auto targetComp =
			dynCast<ecs::Target>(ecs.getComponent(pair.second,
			                                      ecs::Component::Target));
		if(projectileComp && targetComp)
		{
			float pointsWon = projectileComp->getPoints() * targetComp->getPointMultiplier();
				
			points += pointsWon;
			// // Some subtlety here: ballCount_ does not save the target's point
			// // multiplier. It would have an impact if I decide to go back to the
			// // old point system.
			// ++ballCount_[projectileComp->getPoints()];

			updateIndicator(static_cast<int>(round(pointsWon)));
		}
	}

	if(score_ + points > objective_)
	{
		int coins = (points - std::max(0.f, objective_ - score_)) * COINS_PER_EXCESS;
		context_.inventory->addCoins(coins);
	}

	score_ += points;

	std::stringstream ss;
	ss << int(score_);
	ss << "/";
	ss << objective_;
	scoreText_.setString(ss.str());
	sf::Color txtCol = (score_ < objective_) ? sf::Color::Red : sf::Color::Green;
	scoreText_.setColor(txtCol);
}

// Before having the ceiling_'s system, I tried hard not to have gameplay
// pitfalls where the game became too easy or too hard, so here is all the
// documented code in case of getting rid of the ceiling system.
void DifficultyManager::updateObjective()
{
	const int ATTENUATION = 2;
	// const int CUTTING = 2;
	// const int MODULATION = 1;
	const float CEILING_MUL = 1.5f;

	int excess = score_ - objective_;

	if(excess < 0)
	{
		context_.world->setState(World::GameState::GameOver);
		return;
	}
	
	// auto datas = ballDatas;
	// // Eliminated from the excess all the CUTTING most valuable balls.
	// for(int i=0; i<CUTTING; ++i)
	// {
	// 	auto it  = std::max_element(datas.begin(),
	// 	                            datas.end(),
	// 	                            [](BallData data1, BallData data2)
	// 	                            {
	// 		                            return data1.point < data2.point;
	// 	                            });
	// 	assert(it!=ballDatas.end());
	// 	int count = ballCount_.count(it->point);
	// 	excess = std::max(0, excess - it->point*count);
	// 	ballCount_.erase(it->point);
	// 	datas.erase(it);
	// }
	
	// // Try to modulate remaining big points balls : 
	// for(int i=0; i<MODULATION; ++i)
	// {
	// 	auto it  = std::max_element(ballCount_.begin(),
	// 	                            ballCount_.end(),
	// 	                            [](std::pair<const int,int> p1, std::pair<const int,int> p2)
	// 	                            {
	// 		                            return p1.first < p2.first;
	// 	                            });
	// 	if(it != ballCount_.end())
	// 	{
	// 		int ballMaxPoints = (*it).first;
	// 		excess = std::max(0, excess - (ballMaxPoints / 10) * ballCount_[ballMaxPoints]);
	// 	}
	// }

	int increment = OBJECTIVE_INCREMENT + excess / ATTENUATION;
	if (increment >= ceiling_)
	{
		increment = ceiling_;
		float ceiling = ceiling_ * CEILING_MUL;
		ceiling_ = int(ceiling);
	}

	objective_ += increment;
	
	score_ = 0.f;
	// ballCount_.clear();
}

void DifficultyManager::updateIndicatorDuration()
{
	if(indicatorTexts_.size() > 5)
	{
		indicatorDuration_ = INDICATOR_DURATION / 4.f;
	}
	else if(indicatorTexts_.size() > 3)
	{
		indicatorDuration_ = INDICATOR_DURATION / 2.f;
	}
	else
	{
		indicatorDuration_ = INDICATOR_DURATION;
	}
}

void DifficultyManager::updateIndicator(int points)
{
	const unsigned int CHAR_SIZE = 20;
	std::stringstream ss;
	ss << "+" << points;
	sf::Text text (ss.str(), font_, CHAR_SIZE);
	text.setColor(findColor(points));
			
	indicatorTexts_.push_back(text);
	indicatorDeque_.pushBack(&indicatorTexts_.back());
}

sf::Color DifficultyManager::findColor(int points)
{
	auto foundColor =
		std::find_if(
			ballDatas.begin(),
			ballDatas.end(),
			[points](const BallData& data)
			{
				return data.point > points;
			});

	if(foundColor != ballDatas.begin())
	{
		--foundColor;
		return foundColor->color;
	}
	else
	{
		return ballDatas.begin()->color;
	}
}
