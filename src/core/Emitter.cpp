#include "core/Emitter.hpp"
#include "core/World.hpp"


//-----------------------------------------------------------------------------

Emitter::Emitter(World& world)
	: world_(world)
	, emitters_()
{
}


//-----------------------------------------------------------------------------

int Emitter::addParticleEmitter(Particle::Type type, Vector2f& position, unsigned int emissionRate, sf::Color color)
{
	if(type == Particle::TypeCount)
		return -1;

	/* else */
	auto& system = world_.getParticleSystem(type);
	ParticleEmitter emitter (&system, &position, emissionRate, color);
	emitters_.push_back(emitter);

	return emitters_.size() - 1;
}

bool Emitter::removeParticleEmitter(int index)
{
	if(!(-1 < index && index < static_cast<int>(emitters_.size())))
		return false;

	/* else */
	emitters_.erase(emitters_.begin() + index);
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
		emitter.update(dt);


	}
}
