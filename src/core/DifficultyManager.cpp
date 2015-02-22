#include <sstream>

#include "framework/Assertion.hpp"
#include "core/DifficultyManager.hpp"
#include "core/World.hpp"
#include "core/EventGenerator.hpp"


//-----------------------------------------------------------------------------

namespace
{
	Time PHASE_TIME {seconds(20.f)};
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
	, score_(0.f)
	, objective_(baseObjective_)
	, objectiveIncrement_(1)
	, scoreText_()
	, diffGui_(nullptr)
	, maskGui_(true)
	, worldSeed_({0.05f})
	, eventSeed_({100, 8, 10, 10.f})
{

	const Vector2f TIMER_POSITION (720.f, 0.f);
	const Vector2f SCORE_POSITION (720.f, 20.f);
	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	timer_.setFont(font_);
	timer_.setPosition(TIMER_POSITION);
	timer_.setString("0:00");

	scoreText_.setFont(font_);
	scoreText_.setPosition(SCORE_POSITION);
	updateScore();
	
	createGui();

	context_.eventGenerator->updateDifficulty(eventDatas[0]);
}

void DifficultyManager::createGui()
{
	const Vector2f SLIDER_SIZE(40.f,20.f);
	Vector2f sliderPos(0.f,25.f);
	const Vector2f sliderMov(0.f, SLIDER_SIZE.y);
	float i = 0.f;

	gui::Menu::SPtr pMainMenu (new gui::Menu(gui::Menu::Horizontal, "Main Menu", true, true));
	pMainMenu->move(5.f, 5.f);

	//---
	
	gui::Menu::SPtr pGlobalDiff (new gui::Menu(gui::Menu::Vertical, "Global Difficulty"));
	
	gui::Slider<Time>::SPtr pDurationSlider 
		(new gui::Slider<Time>(phaseDuration_, seconds(5.f), SLIDER_SIZE, "Phase Duration", true, seconds(5), seconds(900)));
	pDurationSlider->move(sliderPos);
	pGlobalDiff->pack(std::move(pDurationSlider));
	
	pMainMenu->pack(std::move(pGlobalDiff));

	//---

	
	gui::Menu::SPtr worldDiff (new gui::Menu(gui::Menu::Vertical, "World Difficulty"));
	
	gui::Slider<float>::SPtr pSpeedSlider
		(new gui::Slider<float>(worldSeed_.speedConstant, 0.05f, SLIDER_SIZE, "SpeedCoeff", true, 0.f, 1.f));
	pSpeedSlider->move(sliderPos);
	worldDiff->pack(std::move(pSpeedSlider));

	pMainMenu->pack(std::move(worldDiff));

	//---

	gui::Menu::SPtr eventDiff (new gui::Menu(gui::Menu::Vertical, "Event Difficulty"));

	gui::Slider<float>::SPtr eventDelay
		(new gui::Slider<float>(eventSeed_.delay, 1, SLIDER_SIZE, "Event Delay", true, 0, 60));
	eventDelay->move(sliderPos + i * sliderMov); ++i;
	eventDiff->pack(std::move(eventDelay));

    pMainMenu->pack(std::move(eventDiff)); i=0;

	//---

	gui::Menu::SPtr advEventDiff (new gui::Menu(gui::Menu::Vertical, "Advanced Event Diff"));

	gui::Slider<int>::SPtr attenuation
		(new gui::Slider<int>(eventSeed_.attenuation, 1, SLIDER_SIZE, "Attenuation", true, 1, 16));
	attenuation->move(sliderPos + i * sliderMov); ++i;
	advEventDiff->pack(std::move(attenuation));

	gui::Slider<int>::SPtr offset
		(new gui::Slider<int>(eventSeed_.offset, 2, SLIDER_SIZE, "Offset", true, 0, 30));
    offset->move(sliderPos + i * sliderMov); ++i;
    advEventDiff->pack(std::move(offset));

    pMainMenu->pack(std::move(advEventDiff)); i=0;

	
	//---

	diffGui_ = std::move(pMainMenu);

	// Important
	diffGui_->select();
}

DifficultyManager::~DifficultyManager()
{
}


//-----------------------------------------------------------------------------

void DifficultyManager::update(Time dt)
{
	phaseTime_ += dt;
	updateScore();
	if (phaseTime_ >= phaseDuration_)
	{
		phaseTime_ -= phaseDuration_;
		++phaseNumber_;
		updateDifficulty();
	}

	Time timerRemaining = phaseDuration_ - phaseTime_;
	std::stringstream ssSeconds;
	ssSeconds << int(timerRemaining.asSeconds());
	std::stringstream ssCentiSecs;
	ssCentiSecs << (timerRemaining.asMilliseconds() / 10) % 100;
	timer_.setString(ssSeconds.str() + ":" + ssCentiSecs.str());
}

void DifficultyManager::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased
	   && event.key.code == sf::Keyboard::P)
		reloadDifficulty();

	if(!maskGui_)
	{
		diffGui_->handleEvent(event);
	}
}


void DifficultyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(timer_, states);
	target.draw(scoreText_, states);
	if(!maskGui_)
	{
		target.draw(*diffGui_, states);
	}
}

void DifficultyManager::mask()
{
	maskGui_ = !maskGui_;
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
	auto ecs = context_.world->getEntityManager();
	float points;
	for(const auto& pair : collisions)
	{
		auto projectileComp =
			dynCast<ecs::Projectile>(ecs.getComponent(pair.first,
			                                          ecs::Component::Projectile));
		auto targetComp =
			dynCast<ecs::Target>(ecs.getComponent(pair.second,
			                                      ecs::Component::Target));
		assert(projectileComp);
		assert(targetComp);
		points += projectileComp->getPoints() * targetComp->getPointMultiplier();
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

void DifficultyManager::updateObjective()
{
	const int ATTENUATION = 2;
		
	int excess = score_ - objective_;

	if(excess < 0)
	{
		context_.world->setState(World::GameState::GameOver);
		reset();
		return;
	}
	
	objective_ += objectiveIncrement_ + excess / ATTENUATION;
	score_ = 0.f;
}

void DifficultyManager::reloadDifficulty()
{
	worldDatas = genDifficultyWorld(worldSeed_);
	eventDatas = genDifficultyEvent(eventSeed_);
}

void DifficultyManager::reset()
{
	phaseNumber_ = 0;
	score_ = 0.f;
	objective_ = baseObjective_;
	phaseTime_ = Time::Zero;
	context_.eventGenerator->updateDifficulty(eventDatas[0]);
}
