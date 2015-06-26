#ifndef PARTICLE_AFFECTOR_FUNCTION_HPP
#define PARTICLE_AFFECTOR_FUNCTION_HPP


#include <vector>
#include <functional>
#include <algorithm>

#include "core/Particle.hpp"
#include "data/BallData.hpp"

typedef std::function<void(Particle&, Particle::Type, Time)> ParticleAffector;

namespace AffectorType
{
	enum Type
	{
		FadeOut,
		FadeIn,
//		CustomBallLifetime,
		TypeCount
	};
}

std::vector<ParticleAffector> genParticleAffectors();


#endif // PARTICLE_AFFECTOR_FUNCTION_HPP
