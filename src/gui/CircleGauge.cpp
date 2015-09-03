#include <iostream>

#include "gui/CircleGauge.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		const unsigned int FILLING_N_POINT = 30;

		const sf::Color BACK_VIEW_COLOR (80,80,80);

		const unsigned int SEPARATOR = 5;
		const std::vector<sf::Color> INDICATORS
		{ sf::Color::Red,
		  sf::Color(255,100,0),
		  sf::Color(255,200,0),
		  sf::Color(200,255,0),
		  sf::Color::Green,
		};
	}
	
	CircleGauge::CircleGauge(float radius, float maxValue, float startValue)
		: Gauge(maxValue, startValue)
		, fillingView_(sf::TrianglesFan, FILLING_N_POINT)
		, backgroundView_(radius)
	{
		centerOrigin(backgroundView_);
		backgroundView_.setFillColor(BACK_VIEW_COLOR);
		
		updateFillingView();
	}

	
//-----------------------------------------------------------------------------

	void CircleGauge::updateValue(float value)
	{
		Gauge::updateValue(value);
		updateFillingView();
	}

	void CircleGauge::updateFillingView()
	{
		std::size_t indicatorIndex = 0;
		const float SEPARATION = maxValue_ / static_cast<float>(SEPARATOR);
		while(currentValue_ > SEPARATION * (indicatorIndex+1))
		{
			++indicatorIndex;
		}
		sf::Color color = INDICATORS.at(indicatorIndex);

		fillingView_.clear();
		sf::Vertex toAppend (Vector2f(0.f,0.f), color);
		fillingView_.append(toAppend);
		
		const float PI = 3.1415926535f;
		const float ANGLE_OFFSET = - PI / 2.f;
		const float COUNTER_CLOCKWISE = -1.f;
		float radius = backgroundView_.getRadius() + 0.1f;
		float maxAngle = 2 * PI * (currentValue_ / maxValue_);
		float angleIncrement = maxAngle / FILLING_N_POINT;
		for(unsigned int i=0; i<FILLING_N_POINT+1; ++i)
		{
			float angle = COUNTER_CLOCKWISE * angleIncrement * static_cast<float>(i) + ANGLE_OFFSET;
			toAppend = sf::Vertex(Vector2f(radius * std::cos(angle),
			                               radius * std::sin(angle)),
			                      color);
			fillingView_.append(toAppend);
		}
	}


//-----------------------------------------------------------------------------

	void CircleGauge::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		target.draw(backgroundView_, states);
		target.draw(fillingView_, states);
	}

} // gui::
