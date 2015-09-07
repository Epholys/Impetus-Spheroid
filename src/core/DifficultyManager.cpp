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

const int DifficultyManager::BASE_OBJECTIVE_ = 20;
const Time DifficultyManager::PHASE_TIME_ {seconds(20.f)};


//-----------------------------------------------------------------------------
// *** other constants: ***
namespace
{
	const int BASE_CEILING = 20;
	const int OBJECTIVE_INCREMENT = 2;
	
	const float TIMER_GAUGE_RADIUS = 30.f;
	const Vector2f SCORE_GAUGE_SIZE (15.f, 400.f);
}


//-----------------------------------------------------------------------------

DifficultyManager::DifficultyManager(DifficultyContext context)
	: phaseTime_()
	, phaseNumber_(0)
	, context_(context)
	, font_(context.world->getFontRef())
	, timerGauge_(TIMER_GAUGE_RADIUS, PHASE_TIME_.asSeconds(), PHASE_TIME_.asSeconds())
	, score_(0.f)
	, objective_(BASE_OBJECTIVE_)
	, ceiling_(BASE_CEILING)
	// , scoreText_()
	, scoreGauge_(context.world->getFontRef(), SCORE_GAUGE_SIZE, BASE_OBJECTIVE_ * 4 / 3.f, 0.f)
	, coinsPerPointScore_(context.metaData->improvementValue[ImprovementID::CoinsPerPointScore])
	// , ballCount_()
	// , diffGui_(nullptr)
	// , maskGui_(true)
{
	const Vector2u WINDOW_SIZE = context_.world->getWindowSize();
	const Vector2f TIMER_POSITION (WINDOW_SIZE.x - TIMER_GAUGE_RADIUS - 30.f, TIMER_GAUGE_RADIUS + 10.f);
	const Vector2f SCORE_POSITION (WINDOW_SIZE.x - 55.f, TIMER_GAUGE_RADIUS + 55.f);
	const Vector2f GAUGE_POSITION (WINDOW_SIZE.x - 55.f, TIMER_GAUGE_RADIUS + 95.f);
	timerGauge_.setPosition(TIMER_POSITION);

	scoreGauge_.setPosition(GAUGE_POSITION);
	scoreGauge_.updateStage(BASE_OBJECTIVE_);

	// scoreText_.setFont(font_);
	// scoreText_.setPosition(SCORE_POSITION);
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

	updateScore();

	if (phaseTime_ >= PHASE_TIME_)
	{
		phaseTime_ -= PHASE_TIME_;
		++phaseNumber_;
		updateDifficulty();
	}

	Time timerRemaining = PHASE_TIME_ - phaseTime_;
	timerGauge_.updateValue(timerRemaining.asSeconds());
}

// Used to forward instruction to the GUI 
// void DifficultyManager::handleInput(const sf::Event&)
// {
// }


void DifficultyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(timerGauge_, states);
	// target.draw(scoreText_, states);
	target.draw(scoreGauge_, states);

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
	auto collisions = context_.world->getTrackedCollisions();
	auto& ecs = context_.world->getEntityManager();
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
		}
	}

	if(score_ + points > objective_)
	{
		int coins = (points - std::max(0.f, objective_ - score_)) * coinsPerPointScore_;
		context_.inventory->addCoins(coins);
	}

	score_ += points;

	// std::stringstream ss;
	// ss << int(score_);
	// scoreText_.setString(ss.str());
	// sf::Color txtCol = (score_ < objective_) ? sf::Color::Red : sf::Color::Green;
	// scoreText_.setColor(txtCol);
	scoreGauge_.updateValue(score_);
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

	scoreGauge_.updateStage(objective_);
	scoreGauge_.updateMaxValue(objective_ * 4 / 3.f);
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
