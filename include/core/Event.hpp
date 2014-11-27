#ifndef EVENT_HPP
#define EVENT_HPP


#include <vector>

#include "core/Modifier.hpp"

class Entity;
class World;

namespace evt
{
	struct Event
	{
		Event();

		Time getDuration() const;

		std::vector<Modifier<Entity>> entityModifiers;
		std::vector<Modifier<World>> worldModifiers;

		unsigned int chance;
	};
}


#endif // EVENT_HPP
