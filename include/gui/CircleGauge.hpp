#ifndef GUI_CIRCLE_GAUGE_HPP
#define GUI_CIRCLE_GAUGE_HPP


#include <vector>

#include <SFML/Graphics.hpp>

#include "utility/Vector2.hpp"
#include "utility/utility.hpp"

namespace gui
{

	class CircleGauge : public sf::Drawable, public sf::Transformable
	{
	public:
		CircleGauge(float radius, float maxValue, float startValue = 0.f);

		void updateValue(float value);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	private:
		void updateFillingView();
		
	private:
		float maxValue_;
		float currentValue_;

		sf::VertexArray fillingView_;
		sf::CircleShape backgroundView_;
	};
	
} // gui::



#endif // GUI_CIRCLE_GAUGE_HPP
