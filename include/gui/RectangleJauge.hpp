#ifndef GUI_RECTANGLE_GAUGE_HPP
#define GUI_RECTANGLE_GAUGE_HPP


#include <vector>

#include <SFML/Graphics.hpp>

#include "utility/Vector2.hpp"
#include "utility/utility.hpp"

namespace gui
{

	/* Rectangle jauge tailored for the timer */

	class RectangleGauge : public sf::Drawable, public sf::Transformable
	{
	public:
		RectangleGauge(Vectorf2 size, float maxValue, float startValue = 0.f);

		void updateValue(float value);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void updateFillingView();
		
	private:
		float maxValue_;
		float currentValue_;

		sf::RectangleShape 
		sf::RectangleShape backgroundView_;
	};
	
} // gui::



#endif // GUI_RECTANGLE_GAUGE_HPP
