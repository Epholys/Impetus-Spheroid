#include <iostream>

#include "gui/Gauge.hpp"


//-----------------------------------------------------------------------------

namespace gui
{

	Gauge::Gauge(float maxValue, float startValue)
		: maxValue_(maxValue)
		, currentValue_(startValue)
	{
	}

	Gauge::~Gauge()
	{
	}
	
//-----------------------------------------------------------------------------

	void Gauge::updateValue(float value)
	{
		currentValue_ = std::min(maxValue_, value);
	}

	void Gauge::updateMaxValue(float newMaxValue)
	{
		maxValue_ = newMaxValue;
	}
	
} // gui::

