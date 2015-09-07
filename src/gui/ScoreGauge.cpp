#include <sstream>
#include "framework/Assertion.hpp"
#include "gui/ScoreGauge.hpp"


//-----------------------------------------------------------------------------


namespace gui
{
	namespace
	{
		const sf::Color BACK_VIEW_COLOR (80,80,80);

		const float OUTLINE_THICKNESS = 2.f;

		const sf::Color UNDER_STAGE_COLOR = sf::Color::Red;
		const sf::Color OVER_STAGE_COLOR = sf::Color::Green;

		const float CEILING_FACTOR = 0.8f;
		const float CEILING_UPGRADING_FACTOR = 1.5f;
	}
	
	ScoreGauge::ScoreGauge(sf::Font& font, Vector2f size, float maxValue, float startValue)
		: Gauge(maxValue, startValue)
		, stage_(-1.f)
		, stageView_(Vector2f(size.x, OUTLINE_THICKNESS))
		, stageText_()
		, fillingView_(Vector2f(size.x, 0.f))
		, fillingText_()
		, backgroundView_(size)
	{
		stageView_.setFillColor(UNDER_STAGE_COLOR);

		stageText_.setFont(font);
		stageText_.setColor(UNDER_STAGE_COLOR);
		centerOrigin(stageText_);
		
		backgroundView_.setFillColor(BACK_VIEW_COLOR);
		backgroundView_.setOutlineThickness(OUTLINE_THICKNESS);
		backgroundView_.setOutlineColor(BACK_VIEW_COLOR);

		fillingView_.setOutlineThickness(OUTLINE_THICKNESS);
		fillingView_.setOutlineColor(BACK_VIEW_COLOR);

		fillingText_.setFont(font);
		centerOrigin(fillingText_);
		
		updateViews();
	}
		  
	ScoreGauge::~ScoreGauge()
	{
	}

//-----------------------------------------------------------------------------

	void ScoreGauge::updateStage(float value)
	{
		stage_ = value;
		updateViews();
	}

	void ScoreGauge::updateValue(float value)
	{
		currentValue_ = value;
		if(value > maxValue_ * CEILING_FACTOR)
			updateMaxValue(maxValue_ * CEILING_UPGRADING_FACTOR);
		else
			updateViews();
	}

	void ScoreGauge::updateMaxValue(float value)
	{
		Gauge::updateMaxValue(value);
		updateViews();
	}

	void ScoreGauge::updateViews()
	{
		Vector2f backgroundSize (backgroundView_.getSize());
		
		std::stringstream ss;
		ss << static_cast<int>(stage_);
		stageText_.setString(ss.str());
		Vector2f stageTextPosition (- OUTLINE_THICKNESS -
		                            stageText_.getInverseTransform().transformPoint(stageText_.findCharacterPos(100)).x,
		                            backgroundSize.y * ( 1 - stage_ / maxValue_) -
		                            stageText_.getCharacterSize() / 2.f);
		stageText_.setPosition(stageTextPosition);

		stageView_.setPosition(0.f, backgroundSize.y * ( 1 - stage_ / maxValue_));
		

		assert(maxValue_);
		
		fillingView_.setSize(Vector2f(backgroundSize.x,
		                              backgroundSize.y * (currentValue_ / maxValue_)));
		fillingView_.setPosition(0.f, backgroundSize.y - fillingView_.getSize().y);
		sf::Color color = (currentValue_ < stage_) ? UNDER_STAGE_COLOR : OVER_STAGE_COLOR;
		fillingView_.setFillColor(color);

		ss.str("");
		ss << static_cast<int>(currentValue_);
		fillingText_.setString(ss.str());
		fillingText_.setPosition(fillingView_.getPosition().x - OUTLINE_THICKNESS - fillingText_.getInverseTransform().transformPoint(fillingText_.findCharacterPos(100)).x,
		                         fillingView_.getPosition().y - fillingText_.getCharacterSize() / 2.f);
		fillingText_.setColor(color);
	}				  
	
	void ScoreGauge::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(backgroundView_, states);
		target.draw(fillingView_, states);
		target.draw(fillingText_, states);
		target.draw(stageView_, states);
		target.draw(stageText_, states);
	}

} // gui::
