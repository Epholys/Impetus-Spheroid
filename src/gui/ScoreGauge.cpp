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
	
	ScoreGauge::ScoreGauge(Vector2f size, float maxValue, float startValue)
		: Gauge(maxValue, startValue)
		, stage_(-1.f)
		, stageView_(Vector2f(size.x, OUTLINE_THICKNESS))
		, fillingView_(Vector2f(size.x, 0.f))
		, backgroundView_(size)
	{
		stageView_.setPosition(0.f, size.y * (stage_ / maxValue_));
		stageView_.setFillColor(UNDER_STAGE_COLOR);
		
		backgroundView_.setFillColor(BACK_VIEW_COLOR);
		backgroundView_.setOutlineThickness(OUTLINE_THICKNESS);
		backgroundView_.setOutlineColor(BACK_VIEW_COLOR);

		fillingView_.setOutlineThickness(OUTLINE_THICKNESS);
		fillingView_.setOutlineColor(BACK_VIEW_COLOR);
		
		updateFillingView();
	}
		  
	ScoreGauge::~ScoreGauge()
	{
	}

//-----------------------------------------------------------------------------

	void ScoreGauge::updateStage(float value)
	{
		stage_ = value;
		updateFillingView();
	}

	void ScoreGauge::updateValue(float value)
	{
		currentValue_ = value;
		if(value > maxValue_ * CEILING_FACTOR)
			updateMaxValue(maxValue_ * CEILING_UPGRADING_FACTOR);
		else
			updateFillingView();
	}

	void ScoreGauge::updateMaxValue(float value)
	{
		Gauge::updateMaxValue(value);
		updateFillingView();
	}

	void ScoreGauge::updateFillingView()
	{
		stageView_.setPosition(0.f, backgroundView_.getSize().y * ( 1 - stage_ / maxValue_));

		
		assert(maxValue_);
		
		Vector2f backgroundSize (backgroundView_.getSize());
		fillingView_.setSize(Vector2f(backgroundSize.x,
		                              backgroundSize.y * (currentValue_ / maxValue_)));
		fillingView_.setPosition(0.f, backgroundSize.y - fillingView_.getSize().y);
		sf::Color color = (currentValue_ < stage_) ? UNDER_STAGE_COLOR : OVER_STAGE_COLOR;
		fillingView_.setFillColor(color);
	}				  
	
	void ScoreGauge::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(backgroundView_, states);
		target.draw(fillingView_, states);
		target.draw(stageView_, states);
	}

} // gui::
