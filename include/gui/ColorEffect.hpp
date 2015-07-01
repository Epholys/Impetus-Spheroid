#ifndef GUI_COLOR_EFFECT
#define GUI_COLOR_EFFECT


#include <functional>

#include <SFML/Graphics/Color.hpp>

#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

namespace gui
{
	namespace ColorEffectID
	{
		enum ID
		{
			Transition
		};
	}

	template<typename T>
	class ColorEffect
	{
	public:
		ColorEffect(T* toColor,
		            ColorEffectID::ID type,
		            sf::Color start,
		            sf::Color finish,
		            Time duration);

		void update(Time dt);
		void updateFinish(sf::Color newFinish, Time toAdd = Time());

		bool isOver() const;

	private:
		T* toColor_;
		ColorEffectID::ID type_;
		sf::Color start_;
		sf::Color finish_;
		Time duration_;
		Time accumulatedTime_;
		std::function<sf::Color(Time)> effectFunction_;
	};

	std::function<sf::Color(Time)>
	generateColorEffectFunction(ColorEffectID::ID type,
	                            sf::Color start,
	                            sf::Color finish,
	                            Time duration);
	
	#include "gui/ColorEffect.ipp"
}


#endif // GUI_COLOR_EFFECT
