#ifndef RESOURCE_IDENTIFIERS_HPP
#define RESOURCE_IDENTIFIERS_HPP


#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "framework/ResourceHolder.hpp"

namespace FontID
{
	enum ID
	{
		ForcedSquare,
	};
}

typedef ResourceHolder<sf::Font, FontID::ID> FontHolder;

namespace TextureID
{
	enum ID
	{
		AddTargetbutton,
		AutoFirebutton,
		CancelEventFlash,
		GhostBallbutton,
		NoGravBallbutton,
		AddTarget,
		AutoFire,
		CancelEventsbutton,
		GhostBall,
		NoGravBall,
		AddTimebutton,
		BallTouchDoublebutton,
		CancelEvents,
		MultiplyPointsbutton,
		Particle,
		AddTime,
		BallTouchDouble,
		CrossHair,
		MultiplyPoints,
		PixelParticle,
	};
}

typedef ResourceHolder<sf::Texture, TextureID::ID> TextureHolder;


#endif // RESOURCE_IDENTIFIERS_HPP
