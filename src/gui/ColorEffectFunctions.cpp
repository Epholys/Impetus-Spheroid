#include "gui/ColorEffectFunctions.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	namespace
	{
		std::function<sf::Color(Time)>
		transitionEffect(sf::Color start, sf::Color finish, Time duration)
		{
			float rDiff = finish.r - start.r, gDiff = finish.g - start.g, bDiff = finish.b - start.b, aDiff = finish.a - start.a;
			float seconds = duration.asSeconds();
			rDiff /= seconds; gDiff /= seconds; bDiff /= seconds; aDiff /= seconds;
			
			
			return
				[=](Time dt)
			{
				sf::Color color = start;
				float dtSeconds = dt.asSeconds();
				color.r += rDiff * dtSeconds;
				color.g += gDiff * dtSeconds;
				color.b += bDiff * dtSeconds;
				color.a += aDiff * dtSeconds;
						
				return color;
			};
		}
	}
	
	std::function<sf::Color(Time)>
	generateColorEffectFunction(ColorEffectID::ID type,
	                            sf::Color start,
	                            sf::Color finish,
	                            Time duration)
	{
		switch(type)
		{
		case ColorEffectID::Transition:
			return std::move(transitionEffect(start, finish, duration));
		default:
			return nullptr;
		}
	}

}
