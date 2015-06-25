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

	datas[Particle::Pixel].color = sf::Color::White;
	datas[Particle::Pixel].lifetime = seconds(1.f);
	datas[Particle::Pixel].texturePath = "media/sprites/PixelParticle.png";
	datas[Particle::Pixel].affectors.push_back(AffectorType::FadeOut);
	
	return datas;
}




