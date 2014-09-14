#ifndef EG_ENTITY_HPP
#define EG_ENTITY_HPP


#include <limits>

namespace eg
{
	/* In an Entity/Component System, the Entity is simply a number.
	 * For more information, see http://entity-systems.wikidot.com/
	 * */

	typedef unsigned int Entity;
	Entity maxNEntities = std::numeric_limits<unsigned int>::max();

} // namespace eg


#endif // EG_ENTITY_HPP
