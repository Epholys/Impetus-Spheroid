#ifndef PARTICLE_DATA_HPP
#define PARTICLE_DATA_HPP


#include <string>
#include <vector>
#include <functional>

#include "core/Particle.hpp"
#include "core/ParticleAffectorFunctions.hpp"

struct ParticleData
{
	sf::Color color;
	Time lifetime;
	std::string texturePath;
	std::vector<AffectorType::Type> affectors;
};


std::vector<ParticleData> genParticleData();
	

#endif // PARTICLE_DATA_HPP
