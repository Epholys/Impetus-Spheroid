#include "utility/Time.hpp"

Time seconds(float s)
{
	return sf::seconds(s);
}

Time milliseconds(sf::Int32 ms)
{
	return sf::milliseconds(ms);
}

Time microseconds(sf::Int64 us)
{
	return sf::microseconds(us);
}
