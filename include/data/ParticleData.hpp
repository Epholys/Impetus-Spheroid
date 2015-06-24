#ifndef PARTICLE_DATA_HPP
#define PARTICLE_DATA_HPP


#include <string>
#include <vector>

#include "core/Particle.hpp"

struct ParticleData
{
	sf::Color color;
	Time lifetime;
	std::string texturePath;
};


std::vector<ParticleData> genParticleData();
	

#endif // PARTICLE_DATA_HPP
