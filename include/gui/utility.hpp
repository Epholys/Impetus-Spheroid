#ifndef GUI_UTILITY_HPP
#define GUI_UTILITY_HPP


#include <SFML/Graphics/Text.hpp>

#include "utility/Time.hpp"

namespace gui
{
	// Quick and dangerous, but better than a useless behemoth
	template<typename T>
	struct FadeOut
	{
		T* pT_;
		Time duration_;
		Time accumulatedTime_;

		void update(Time dt);
	};

	#include "gui/utility.ipp"
}

#endif // GUI_UTILITY_HPP
