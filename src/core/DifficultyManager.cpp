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
	, diffGui_(nullptr)
	, worldSeed_()
{
	const Vector2f TIMER_POSITION (720.f, 20.f);

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	timer_.setFont(font_);
	timer_.setPosition(TIMER_POSITION);
	timer_.setString("0:00");

	worldSeed_.speedConstant = 0.05f;

	createGui();
}

void DifficultyManager::createGui()
{
	const Vector2f SLIDER_SIZE(40.f,20.f);
	Vector2f sliderPos(0.f,25.f);
	const Vector2f sliderMov(0.f, SLIDER_SIZE.y);

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

void DifficultyManager::handleInput(const sf::Event& event)
{
	if(event.type == sf::Event::KeyReleased
	   && event.key.code == sf::Keyboard::P)
		reloadDifficulty();

	else if(event.type == sf::Event::KeyReleased
	   && event.key.code == sf::Keyboard::O)
		reset();

	diffGui_->handleEvent(event);
}

void DifficultyManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(timer_, states);
	target.draw(*diffGui_, states);
}


//-----------------------------------------------------------------------------

void DifficultyManager::updateDifficulty()
{
	auto worldIndex = selectDataIndex(worldDatas);
	context_.world->updateDifficulty(worldDatas[worldIndex]);
	// context_.eventGenerator->update(data);
}

void DifficultyManager::reloadDifficulty()
{
	worldDatas = genDifficultyWorld(worldSeed_);
}

void DifficultyManager::reset()
{
	phaseNumber_ = 0;
	phaseTime_ = Time::Zero;
}
