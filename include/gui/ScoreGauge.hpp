#ifndef GUI_SCORE_GAUGE_HPP
#define GUI_SCORE_GAUGE_HPP


#include "gui/Gauge.hpp"
#include "utility/Vector2.hpp"
#include "utility/utility.hpp"

namespace gui
{

	/* Score gauge tailored for the score gauge */

	class ScoreGauge : public Gauge
	{
	public:
		ScoreGauge(Vector2f size, float maxValue, float startValue = 0.f);
		virtual ~ScoreGauge();

		virtual void updateStage(float value);
		virtual void updateValue(float value);
		virtual void updateMaxValue(float value);
		
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:
		virtual void updateFillingView();
					
	protected:
		float stage_;

		sf::RectangleShape fillingView_;
		sf::RectangleShape backgroundView_;
	};
	
} // gui::



#endif // GUI_SCORE_GAUGE_HPP
