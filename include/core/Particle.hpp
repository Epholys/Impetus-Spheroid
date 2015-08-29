#ifndef PARTICLE_HPP
#define PARTICLE_HPP


#include <SFML/Graphics/Color.hpp>

#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

/* Simple Particle Struct, no information about Texture since it must be very
 * light, the number of particle can be very high
 * */

struct Particle
{
	enum Type
	{
		BallTrail,
		Shard,
		Bubble,
		TypeCount
	};
	
	Vector2f position;
	Vector2f velocity;
	float angle;
	float scale;
	sf::Color color;
	Time lifetime;
};


#endif // PARTICLE_HPP
