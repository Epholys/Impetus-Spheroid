#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP


#include "core/ParticleSystem.hpp"

class ParticleEmitter
{
public:
	ParticleEmitter(ParticleSystem* system, Vector2f* position, unsigned int emissionRate, sf::Color color);

	void update(Time dt);

private:
	ParticleSystem* system_;

	Time accumulatedTime_;
	Vector2f* position_;
	Vector2f previousPosition_;
	Time frequency_;
	sf::Color color_;
};


#endif // PARTICLE_EMITTER_HPP
