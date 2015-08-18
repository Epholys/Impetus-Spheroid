#include "data/ParticleData.hpp"


//-----------------------------------------------------------------------------


std::vector<ParticleData> genParticleData ()
{
	std::vector<ParticleData> datas (Particle::TypeCount);

	// Default Color, must be personnalized during creation of the Particle via
	// ParticleSystem::emitParticle().
	datas[Particle::BallTrail].color = sf::Color::White;
	datas[Particle::BallTrail].lifetime = seconds(0.1f);
	datas[Particle::BallTrail].texturePath = "media/sprites/Particle.png";
	datas[Particle::BallTrail].affectors.push_back(AffectorType::FadeOut);
	// datas[Particle::BallTrail].affectors.push_back(AffectorType::CustomBallLifetime);

	datas[Particle::Shard].color = sf::Color::White;
	datas[Particle::Shard].lifetime = seconds(1.f);
	datas[Particle::Shard].texturePath = "media/sprites/PixelParticle.png";
	datas[Particle::Shard].affectors.push_back(AffectorType::FadeOut);

	datas[Particle::Bubble].color = sf::Color::White;
	datas[Particle::Bubble].lifetime = seconds(1.75f);
	datas[Particle::Bubble].texturePath = "media/sprites/Particle.png";
	datas[Particle::Bubble].affectors.push_back(AffectorType::FadeIn);
	
	return datas;
}
