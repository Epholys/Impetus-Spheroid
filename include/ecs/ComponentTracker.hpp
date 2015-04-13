#ifndef ECS_COMPONENTTRACKER_HPP
#define ECS_COMPONENTTRACKER_HPP


#include "ecs/ComponentBase.hpp"

namespace ecs
{

	/* Here are the two Components which tracks the collisions: Projectile and
	 * Target. When a Projectile touch a Target, it is recorded in the
	 * PhysicEngine. A Projectile can only touch one Target.
	 * 
	 * For more information about a Entity/Component System, see:
	 * http://entity-systems.wikidot.com/
	 * */

//-----------------------------------------------------------------------------

	class Target : public ComponentBase
	{
	public:
		Target(float pointMultiplier);

		/* These two functions are used to keep track of wether or not the
		 * target has been touched, for the purpose of moving it, for example,
		 * and not to keep track of each collision separately.
		 * */
		void touch();
		bool reset();

		float getPointMultiplier() const;
		void setPointMultiplier(float pMul);

		virtual Component::Category getCategory() const;

	private:
		bool hasBeenTouched_;
		float pointMultiplier_;
	};

//-----------------------------------------------------------------------------

	class Projectile : public ComponentBase
	{
	public:
		Projectile(int points, int nTouching);
		
		void touchTarget();
		bool hasTouchedTarget() const;

		int getPoints() const;

		virtual Component::Category getCategory() const;

	private:
		bool hasTouchedTarget_;
		int nTouching_;
		int points_;
	};


} // namespace ecs


#endif // ECS_COMPONENTTRACKER_HPP
