#include <sstream>

#include "core/DifficultyManager.hpp"
#include "core/World.hpp"
#include "core/EventGenerator.hpp"


//-----------------------------------------------------------------------------

namespace
{
	Time PHASE_TIME {seconds(10.f)};
	auto worldDatas = genDifficultyWorld();
	auto eventDatas = genDifficultyEvent();
}

//-----------------------------------------------------------------------------

DifficultyManager::DifficultyManager(DifficultyContext context)
	: phaseDuration_(PHASE_TIME)
	, phaseTime_()
	, phaseNumber_(0)
	, context_(context)
	, font_()
	, timer_()
{
	const Vector2f TIMER_POSITION (0.f, 20.f);

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	timer_.setFont(font_);
	timer_.setPosition(TIMER_POSITION);
	timer_.setString("0:00");
}

DifficultyManager::~DifficultyManager()
{
}


//-----------------------------------------------------------------------------

void DifficultyManager::update(Time dt)
{
	phaseTime_ += dt;
	if (phaseTime_ >= phaseDuration_)
	{
		phaseTime_ -= phaseDuration_;
		updateDifficulty();
		++phaseNumber_;
	}

	Time timerRemaining = phaseDuration_ - phaseTime_;
	std::stringstream ssSeconds;
	ssSeconds << int(timerRemaining.asSeconds());
	std::stringstream ssCentiSecs;
	ssCentiSecs << (timerRemaining.asMilliseconds() / 10) % 100;
	timer_.setString(ssSeconds.str() + ":" + ssCentiSecs.str());
}

void DifficultyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(timer_, states);
}


//-----------------------------------------------------------------------------

void DifficultyManager::updateDifficulty()
{
	auto worldIndex = selectDataIndex(worldDatas);
	context_.world->updateDifficulty(worldDatas[worldIndex]);
	// context_.eventGenerator->update(data);
}
