#include "gui/utility.hpp"


//-----------------------------------------------------------------------------

namespace gui
{
	void FadeOutText::update(Time dt)
	{
		if(!pText_)
			return;
		
		if(accumulatedTime_ >= duration_)
		{
			pText_->setColor(sf::Color(0,0,0,0));
			return;
		}

		accumulatedTime_ += dt;

		sf::Color color = pText_->getColor();
		float a = std::max(0.f, 255 - (255 / duration_.asSeconds()) * accumulatedTime_.asSeconds());
		color.a = static_cast<sf::Uint8>(a);
		pText_->setColor(color);
	}
}
