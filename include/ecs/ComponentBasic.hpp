#ifndef ECS_COMPONENTBASIC_HPP
#define ECS_COMPONENTBASIC_HPP


#include "utility/Vector2.hpp"
#include "ecs/ComponentBase.hpp"

namespace ecs
{

	/* Here are the two basics Components : Position and Velocity 
	 *
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */

//-----------------------------------------------------------------------------

	class Position : public ComponentBase
	{
	public:
		explicit Position(Vector2f pos = Vector2f(0.f,0.f));

		virtual Component::Category getCategory() const;

	public:
		Vector2f position_;
	};

//-----------------------------------------------------------------------------

	class Velocity : public ComponentBase
	{
	public:
		explicit Velocity(Vector2f vel = Vector2f(0.f,0.f));

		virtual Component::Category getCategory() const;

	public:
		Vector2f velocity_;
	};

} // namespace ecs


#endif // ECS_COMPONENTBASIC_HPP
