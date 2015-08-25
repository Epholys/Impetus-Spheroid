#ifndef ECS_COMPONENTPHYSIC_HPP
#define ECS_COMPONENTPHYSIC_HPP


#include "utility/Vector2.hpp"
#include "ecs/ComponentBase.hpp"

namespace ecs
{

	/* Here are all the Components about physics. That includes:
	 * - time flow control with TimeArrow
	 * - gravity with Mass
	 * - bounce with Solid
	 * - collision with Collidable class
	 *
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */


//-----------------------------------------------------------------------------

	class TimeArrow : public ComponentBase
	{
	public:
		TimeArrow(float coefficient);

		virtual Component::Category getCategory() const;

		float timeCoefficient_;
	};

	
//-----------------------------------------------------------------------------

	class Mass : public ComponentBase
	{
	public:
		Mass(float mass, Vector2f gravity);

		virtual Component::Category getCategory() const;

		float mass_;
		Vector2f gravityVect_;
	};

//-----------------------------------------------------------------------------

	class Solid : public ComponentBase
	{
	public:
		/* pre-condidition : rest must be positive or equals to 0.
		 * If not, it will be set to 0. */
		Solid(float invMass, float rest);

		virtual Component::Category getCategory() const;

	public:
		float invMass_;
		float restitution_;
	};


//-----------------------------------------------------------------------------

	/* Not a very OOP class with the use of union and Type... It is just to
	 * avoid unecessary subclasses
	 * */
	class Collidable : public ComponentBase
	{
	public:
		enum Type
		{
			None = 0,
			Sphere,
			Rectangle,
		};

		union
		{
			float radius;
			Vector2f size;
		};

	public:
		Collidable();
		explicit Collidable(Type type);

		virtual Component::Category getCategory() const;

	public:
		Type type;
	};

} // namespace ecs


#endif // ECS_COMPONENTPHYSIC_HPP
