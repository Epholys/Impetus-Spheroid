#ifndef GUI_CIRCLE_GAUGE_HPP
#define GUI_CIRCLE_GAUGE_HPP


#include <vector>

#include "gui/Gauge.hpp"
#include "utility/Vector2.hpp"
#include "utility/utility.hpp"

namespace gui
{

	/* Circle jauge tailored for the timer */

	class CircleGauge : public Gauge
	{
	public:
		CircleGauge(float radius, float maxValue, float startValue = 0.f);

		virtual void updateValue(float value);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	protected:
		virtual void updateFillingView();
		
	protected:
		sf::VertexArray fillingView_;
		sf::CircleShape backgroundView_;
	};
	
} // gui::



#endif // GUI_CIRCLE_GAUGE_HPP
