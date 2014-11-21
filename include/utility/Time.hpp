#ifndef UT_TIME_HPP
#define UT_TIME_HPP


#include <SFML/System/Time.hpp>

/* Normally, that would be a custom class for all the ECS, the Engine, and
 * everything else, but SFML did such a good and minimalist job that I can't
 * resist to use it. It should be replaceable easily.
 *
 * For more information, visit http://sfml-dev.org/
 * */

typedef sf::Time Time;

Time seconds(float s);

Time milliseconds(sf::Int32 ms);

Time microseconds(sf::Int64 us);

#endif // UT_TIME_HPP
