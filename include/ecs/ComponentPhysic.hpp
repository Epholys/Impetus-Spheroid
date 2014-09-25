#ifndef ECS_COMPONENTPHYSIC_HPP
#define ECS_COMPONENTPHYSIC_HPP


#include "utility/Vector2.hpp"
#include "ecs/ComponentBase.hpp"

namespace ecs
{

	/* Here are all the Components about physics. That includes:
	 * - gravity with Mass
	 * - bounce with Solid
	 * - collision with Collidable base class and its children class
	 *
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */

//-----------------------------------------------------------------------------

	class Mass : public ComponentBase
	{
	public:
		explicit Mass();

		virtual Component::Category getCategory() const;
	};

//-----------------------------------------------------------------------------

	class Solid : public ComponentBase
	{
	public:
		/* pre-condidition : rest must be positive or equals to 0.
		 * If not, it will be set to 0. */
		explicit Solid(float rest);

		virtual Component::Category getCategory() const;

	public:
		float restitution_;
	};


//-----------------------------------------------------------------------------

	class Collidable : public ComponentBase
	{
		// Base class
	};

	class CollidableSphere : public Collidable
	{
	public:
		// A negative radius is ok.
		explicit CollidableSphere(float radius);

		virtual Component::Category getCategory() const;

	public:
		float radius_;
	};


	class CollidableRect : public Collidable
	{
	public:
		// A negative size is ok.
		explicit CollidableRect(Vector2f size);

		virtual Component::Category getCategory() const;

	public:
		Vector2f size_;
	};

	

} // namespace ecs


#endif // ECS_COMPONENTPHYSIC_HPP