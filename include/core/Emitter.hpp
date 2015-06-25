#ifndef EMITTER_HPP
#define EMITTER_HPP


#include <unordered_map>

#include "core/ParticleEmitter.hpp"

class World;

class Emitter
{
public:
	Emitter(World& world);

	int addParticleEmitter(Particle::Type type, Vector2f& position, unsigned int emissionRate, sf::Color color, Vector2f velocity = Vector2f(0.f, 0.f));

	bool removeParticleEmitter(int index);
	void removeAllParticleEmitters();

	void update(Time dt);
	
private:
	World& world_;
	std::unordered_map<unsigned int, ParticleEmitter> emitters_;
	int emitterCount_;
};



#endif // EMITTER_HPP
