#ifndef UT_UTILITY_HPP
#define UT_UTILITY_HPP


#include <memory>
#include <vector>
#include <sstream>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Window/Keyboard.hpp>

#include "utility/Vector2.hpp"
#include "ecs/ComponentBase.hpp"

/* Just to have a shorter name that
 * std::dynamic_pointer_cast<...>(ecs::ComponentBase)   
 * */
template<typename Child>
std::shared_ptr<Child> dynCast(const ecs::ComponentBase::SPtr& pParent)
{
	return std::dynamic_pointer_cast<Child>(pParent);
}

std::string	toString(sf::Keyboard::Key key);

template<typename T>
std::string toString(const T& var)
{
	std::stringstream ss;
	ss << var;
	return ss.str();
}

//------------------------------------------------------------------------------

/* For all sf::Transformable which has a getLocalBounds() function.
 * */
template<typename T>
void centerOrigin(T& t)
{
	sf::FloatRect bounds = t.getLocalBounds();
	t.setOrigin(bounds.left + bounds.width / 2.f,
	            bounds.top + bounds.height / 2.f);
}


void defineText(sf::Text& txt,
                const std::string& str,
                Vector2f position,
                sf::Color color = sf::Color::White,
                bool centerOrigin = true);

//-----------------------------------------------------------------------------

float round (float toRound);

#endif // UT_UTILITY_HPP
