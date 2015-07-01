#ifndef GUI_COLOR_EFFECT_FUNCTIONS_HPP
#define GUI_COLOR_EFFECT_FUNCTIONS_HPP


#include "gui/ColorEffect.hpp"

namespace gui
{
	std::function<sf::Color(Time)>
	generateColorEffectFunction(ColorEffectID::ID type,
	                            sf::Color start,
	                            sf::Color finish,
	                            Time duration);
}



#endif // GUI_COLOR_EFFECT_FUNCTIONS_HPP
