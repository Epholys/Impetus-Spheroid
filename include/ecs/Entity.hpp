#ifndef ECS_ENTITY_HPP
#define ECS_ENTITY_HPP


#include <limits>

namespace ecs
{
	/* In an Entity/Component System, the Entity is simply a number.
	 * For more information, see http://entity-systems.wikidot.com/
	 * */

	typedef unsigned int Entity;
	Entity maxNEntities = std::numeric_limits<unsigned int>::max();

} // namespace ecs


#endif // ECS_ENTITY_HPP
