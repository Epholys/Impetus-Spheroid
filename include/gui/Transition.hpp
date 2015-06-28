#ifndef GUI_TRANSITION
#define GUI_TRANSITION


#include <functional>

#include <SFML/Graphics/Transformable.hpp>

#include "utility/Vector2.hpp"
#include "utility/Time.hpp"

namespace gui
{
	class Transition
	{
	public:
		enum Type
		{
			Linear
		};

	public:
		Transition(sf::Transformable* toMove,
		           Type type,
		           Vector2f start,
		           Vector2f finish,
		           Time duration);

		void update(Time dt);
		void updateFinish(Vector2f newFinish, Time toAdd = Time());

		bool isOver() const;

	private:
		sf::Transformable* toMove_;
		Type type_;
		const Vector2f start_;
		Vector2f finish_;
		Time duration_;
		Time accumulatedTime_;
		std::function<Vector2f(Time)> transitionFunction_;
	};
}


#endif // GUI_TRANSITION
