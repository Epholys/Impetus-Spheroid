#include "data/ParticleData.hpp"


//-----------------------------------------------------------------------------


std::vector<ParticleData> genParticleData ()
{
	std::vector<ParticleData> datas (Particle::TypeCount);

	// Default Color, must be personnalized during creation of the Particle via
	// ParticleSystem::emitParticle().
	datas[Particle::BallTrail].color = sf::Color::White;
	datas[Particle::BallTrail].lifetime = seconds(0.075f);
	datas[Particle::BallTrail].texturePath = "media/sprites/Particle.png";
	datas[Particle::BallTrail].affectors.push_back(AffectorType::FadeOut);
	// datas[Particle::BallTrail].affectors.push_back(AffectorType::CustomBallLifetime);
	
	return datas;
}
