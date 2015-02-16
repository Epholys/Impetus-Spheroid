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
	const Vector2f TIMER_POSITION (750.f, 20.f);

	font_.loadFromFile("./media/font/FORCEDSQUARE.ttf");

	timer_.setFont(font_);
	timer_.setPosition(TIMER_POSITION);
	timer_.setString("0:00");

	worldSeed_.speedCoeff = 0.05f;

	createGui();
}

void DifficultyManager::createGui()
{
	const Vector2f SLIDER_SIZE(40.f,20.f);
	Vector2f sliderPos(0.f,25.f);
	const Vector2f sliderMov(0.f, SLIDER_SIZE.y);

//	gui::Menu* mainMenu = new gui::Menu(gui::Menu::Horizontal, true, true);
	
	gui::Menu* globalDiff = new gui::Menu(gui::Menu::Vertical);
	
	gui::Slider<Time>::SPtr durationSlider 
		(new gui::Slider<Time>(phaseDuration_, seconds(5.f), SLIDER_SIZE, "Phase Duration", true, seconds(5), seconds(900)));
	durationSlider->move(sliderPos);	sliderPos+=sliderMov;
	gui::Slider<Time>::SPtr pDurationSlider (durationSlider);
	globalDiff->pack(pDurationSlider);
	
	gui::Slider<float>::SPtr pSpeedSlider
		(new gui::Slider<float>(worldSeed_.speedCoeff, 0.05f, SLIDER_SIZE, "SpeedCoeff", true, 0.f, 1.f));
	pSpeedSlider->move(sliderPos);	sliderPos+=sliderMov;
	globalDiff->pack(pSpeedSlider);
	
	gui::Menu::SPtr pGlobalDiff (globalDiff);
//	mainMenu->pack(pGlobalDiff);

//	gui::Menu::SPtr pMainMenu (mainMenu);
	gui::Menu::SPtr pMainMenu (pGlobalDiff);
	
	diffGui_ = pMainMenu;
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
