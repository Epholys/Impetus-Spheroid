#ifndef EMITTER_HPP
#define EMITTER_HPP


#include <unordered_map>

#include "core/ParticleEmitter.hpp"

class World;

class Emitter
{
public:
	Emitter(World& world);

	int addParticleEmitter(Particle::Type type, Vector2f position, float emissionRate, sf::Color color, Vector2f velocity = Vector2f(0.f, 0.f), float angle = 0.f, float scale = 1.f);

	bool removeParticleEmitter(int index);
	void removeAllParticleEmitters();

	void update(Time dt);
	bool updatePosition(int index, Vector2f position);
	
private:
	World& world_;
	std::unordered_map<unsigned int, ParticleEmitter> emitters_;
	int emitterCount_;
};



#endif // EMITTER_HPP
