#ifndef GUI_TEXTURE_GENERATOR_HPP
#define GUI_TEXTURE_GENERATOR_HPP


#include <sstream>
#include <memory>

#include <SFML/Graphics.hpp>

#include "utility/utility.hpp"
#include "framework/Assertion.hpp"

namespace gui
{
	namespace TextureGenerator
	{
		std::unique_ptr<sf::Texture> marketButton(unsigned int width,
		                                          unsigned int height,
		                                          const std::string& icon,
		                                          const std::string& fontPath,
		                                          int number,
		                                          const std::string& legend,
		                                          int price,
		                                          const std::string& shortcut);
	}
}


#endif // GUI_TEXTURE_GENERATOR_HPP
