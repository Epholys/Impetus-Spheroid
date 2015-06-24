#include "data/ParticleData.hpp"


//-----------------------------------------------------------------------------

std::vector<ParticleData> genParticleData ()
{
	std::vector<ParticleData> datas (Particle::TypeCount);

	// Default Color, must be personnalized during creation of the Particle via
	// ParticleSystem::emitParticle().
	datas[Particle::BallTrail].color = sf::Color::White;
	datas[Particle::BallTrail].lifetime = seconds(0.05f);
	datas[Particle::BallTrail].texturePath = "media/sprites/Particle.png";

	return datas;
}
