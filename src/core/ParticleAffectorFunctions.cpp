#include "core/ParticleAffectorFunctions.hpp"
#include "data/ParticleData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto datas = genParticleData();
//	auto ballDatas = genBallDatas();
	
	auto fadeOut =
		[&](Particle& p, Particle::Type type, Time)
		{
			float ratio = p.lifetime.asSeconds() / datas[type].lifetime.asSeconds();
			p.color.a = static_cast<sf::Uint8>(p.color.a * std::max(0.f, ratio));
		};

	auto quadFadeOut =
		[&](Particle& p, Particle::Type type, Time)
		{
			float duration = datas[type].lifetime.asSeconds();
			float accTime = duration - p.lifetime.asSeconds();
			p.color.a = static_cast<sf::Uint8>
				(std::max(0.f,
				          255 - 255 * std::pow(accTime/duration, 2.f)));
		};

	
	auto fadeIn =
		[&](Particle& p, Particle::Type type, Time)
		{
			float ratio = (datas[type].lifetime.asSeconds() - p.lifetime.asSeconds()) / datas[type].lifetime.asSeconds();
			p.color.a = static_cast<sf::Uint8>(255 * std::max(0.f, ratio));
		};

	auto rotation =
		[&](float angularVelDegree, Particle& p, Particle::Type, Time dt)
	{
		p.angle += angularVelDegree * dt.asSeconds();
	};

	/* Removed because the effects is only visible with really hight points
	 * balls (50 and 100). I could make a custom table but I think the ball's
	 * value will be better visualized with particule effects on the pad.
	 * */

	// auto customBallLifetime =
	// 	[&](Particle& p, Particle::Type type, Time dt)
	// 	{
	// 		if (type != Particle::BallTrail)
	// 			return;

	// 		auto gotData = std::find_if(ballDatas.begin(),
	// 		                            ballDatas.end(),
	// 		                            [&](const BallData& data)
	// 		                            {return p.color.r == data.color.r &&
	// 		                                    p.color.g == data.color.g &&
	// 		                                    p.color.b == data.color.b;});

	// 		if(gotData == ballDatas.end())
	// 		   return;

	// 		float percentToAdd = gotData->point / 110.f;
	// 		p.lifetime += dt * percentToAdd;
	// 	};
}
			


//-----------------------------------------------------------------------------

std::vector<ParticleAffector> genParticleAffectors()
{
	std::vector<ParticleAffector> affectors (AffectorType::TypeCount);
	
	affectors[AffectorType::FadeOut] = fadeOut;
	affectors[AffectorType::QuadraticFadeOut] = quadFadeOut;
	affectors[AffectorType::FadeIn] = fadeIn;
	affectors[AffectorType::Rotation] = std::bind(rotation, 720, std::placeholders::_1,std::placeholders::_2, std::placeholders::_3);
	// affectors[AffectorType::CustomBallLifetime] = customBallLifetime;

	return affectors;
}
