#ifndef EVENT_HPP
#define EVENT_HPP


#include <vector>

#include "core/Modifier.hpp"

class Entity;
class World;

namespace evt
{
	/* An event is just a collection of (Modifiable<> classes') modifiers. There
	 * is just a getDuration() utility function. chance and diff are used in
	 * EventGenerator. 
	 *
	 * */

	struct Event
	{
		enum Difficulty
		{
			None=0, Easy, Medium, Hard
		};

		Event();

		Time getDuration() const;

		std::vector<Modifier<Entity>> entityModifiers;
		std::vector<Modifier<World>> worldModifiers;

		unsigned int chance;
		Difficulty diff;
	};
}


#endif // EVENT_HPP
