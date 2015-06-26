#include "core/Emitter.hpp"
#include "core/World.hpp"


//-----------------------------------------------------------------------------

Emitter::Emitter(World& world)
	: world_(world)
	, emitters_()
	, emitterCount_(0)
{
}


//-----------------------------------------------------------------------------

int Emitter::addParticleEmitter(Particle::Type type, Vector2f position, float emissionRate, sf::Color color, Vector2f velocity)
{
	if(type == Particle::TypeCount)
		return -1;

	/* else */
	auto& system = world_.getParticleSystem(type);
	ParticleEmitter emitter (&system, position, velocity, emissionRate, color);
	emitters_.emplace(emitterCount_, emitter);
	++emitterCount_;

	return emitterCount_ - 1;
}

bool Emitter::removeParticleEmitter(int index)
{
	if(!(-1 < index && index < emitterCount_))
		return false;

	/* else */
	emitters_.erase(index);
	return true;
}

void Emitter::removeAllParticleEmitters()
{
	emitters_.clear();
}


//-----------------------------------------------------------------------------

void Emitter::update(Time dt)
{
	for(auto& emitter : emitters_)
	{
		emitter.second.update(dt);
	}
}

bool Emitter::updatePosition(int index, Vector2f position)
{
	if(!(-1 < index && index < emitterCount_))
		return false;

	/* else */
	emitters_.at(index).updatePosition(position);
	return true;
}
