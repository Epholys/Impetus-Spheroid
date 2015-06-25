#include "core/ParticleAffectorFunctions.hpp"
#include "data/ParticleData.hpp"


//-----------------------------------------------------------------------------

namespace
{
	auto datas = genParticleData();
	// auto ballDatas = genBallDatas();
	
	auto fadeOut =
		[&](Particle& p, Particle::Type type, Time)
		{
			float ratio = p.lifetime.asSeconds() / datas[type].lifetime.asSeconds();
			p.color.a = static_cast<sf::Uint8>(p.color.a * std::max(0.f, ratio));
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
	// affectors[AffectorType::CustomBallLifetime] = customBallLifetime;

	return affectors;
}
