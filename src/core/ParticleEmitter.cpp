#include "core/ParticleEmitter.hpp"


//-----------------------------------------------------------------------------

ParticleEmitter::ParticleEmitter(ParticleSystem* system, Vector2f* position, Vector2f velocity, unsigned int emissionRate, sf::Color color)
	: system_(system)
	, position_(position)
	, previousPosition_(*position)
	, velocity_(velocity)
	, frequency_(seconds(1.f))
	, color_(color)
{
	if(emissionRate != 0)
	{
		frequency_ = seconds(1.f / emissionRate);
	}
}


//-----------------------------------------------------------------------------

void ParticleEmitter::update(Time dt)
{
	accumulatedTime_ += dt;

	if(accumulatedTime_ < frequency_ || !system_ || !position_)
		return;

	/*else*/
	float nParticuleToEmit = ((accumulatedTime_ - frequency_) / frequency_.asSeconds()).asSeconds();

	if(std::ceil(nParticuleToEmit) == 0)
		return;

	/*else*/
	Vector2f gap = *position_ - previousPosition_;
	Vector2f fillingDistance = gap / float(nParticuleToEmit);
	Vector2f particulePosition = previousPosition_;
	for(int i=0; i<nParticuleToEmit; ++i)
	{
		accumulatedTime_ -= frequency_;
		system_->addParticle(particulePosition, velocity_, color_);
		particulePosition += fillingDistance;
	}
	
	previousPosition_ = *position_;
}
