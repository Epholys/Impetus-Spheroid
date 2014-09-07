#ifndef EG_TIME_HPP
#define EG_TIME_HPP


#include <SFML/System/Time.hpp>

namespace eg
{
	/* Normally, this would be a custom class, as the physic engine should be as
	 * independent as possible. However, sf::Time is minimalist and can be
	 * simply replaced by an other implementation.
	 * For more information, see http://www.sfml-dev.org/index.php
	 * */

	typedef sf::Time Time;

} // namespace eg


#endif // EG_TIME_HPP
