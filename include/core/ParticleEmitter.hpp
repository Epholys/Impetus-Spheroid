#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP


#include "core/ParticleSystem.hpp"

class ParticleEmitter
{
public:
	ParticleEmitter(ParticleSystem* system, Vector2f position, Vector2f velocity, float emissionRate, sf::Color color);

	void update(Time dt);
	void updatePosition(Vector2f position);

private:
	ParticleSystem* system_;

	Time accumulatedTime_;
	Vector2f position_;
	Vector2f previousPosition_;
	Vector2f velocity_;
	Time frequency_;
	sf::Color color_;
};


#endif // PARTICLE_EMITTER_HPP
