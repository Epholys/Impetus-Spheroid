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
	}
	
	ScoreGauge::ScoreGauge(Vector2f size, float maxValue, float startValue)
		: Gauge(maxValue, startValue)
		, stage_(-1.f)
		, fillingView_(Vector2f(size.x, 0.f))
		, backgroundView_(size)
	{
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
		Gauge::updateValue(value);
		updateFillingView();
	}

	void ScoreGauge::updateMaxValue(float value)
	{
		Gauge::updateMaxValue(value);
		updateFillingView();
	}

	void ScoreGauge::updateFillingView()
	{
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
	}

} // gui::
