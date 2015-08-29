#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP


#include "core/ParticleSystem.hpp"

class ParticleEmitter
{
public:
	ParticleEmitter(ParticleSystem* system, Vector2f position, Vector2f velocity, float emissionRate, sf::Color color, float angle = 0.f, float scale = 1.f);

	void update(Time dt);
	void updatePosition(Vector2f position);

private:
	ParticleSystem* system_;

	Time accumulatedTime_;
	Vector2f position_;
	Vector2f previousPosition_;
	Vector2f velocity_;
	float angle_;
	float scale_;
	Time frequency_;
	sf::Color color_;
};


#endif // PARTICLE_EMITTER_HPP
