#ifndef GAUGE_HPP
#define GAUGE_HPP


#include <SFML/Graphics.hpp>

namespace gui
{

	class Gauge : public sf::Drawable, public sf::Transformable
	{
	public:
		Gauge(float maxValue, float startValue = 0.f);
		virtual ~Gauge();

		virtual void updateValue(float value);
		virtual void updateMaxValue(float newMaxValue);

	protected:
		float maxValue_;
		float currentValue_;
	};
	
} // gui::


#endif // GAUGE_HPP
