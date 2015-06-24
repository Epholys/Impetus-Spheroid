#ifndef EMITTER_HPP
#define EMITTER_HPP


#include <vector>

#include "core/ParticleEmitter.hpp"

class World;

class Emitter
{
public:
	Emitter(World& world);

	int addParticleEmitter(Particle::Type type, Vector2f& position, unsigned int emissionRate, sf::Color color);

	bool removeParticleEmitter(int index);
	void removeAllParticleEmitters();

	void update(Time dt);
	
private:
	World& world_;
	std::vector<ParticleEmitter> emitters_;
};



#endif // EMITTER_HPP
