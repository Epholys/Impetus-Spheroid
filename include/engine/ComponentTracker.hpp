#ifndef EG_COMPONENTTRACKER_HPP
#define EG_COMPONENTTRACKER_HPP


#include "ComponentBase.hpp"

namespace eg
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
		Target()

		virtual Component::Category getCategory() const;
	};

//-----------------------------------------------------------------------------

	class Projectile : public ComponentBase
	{
	public:
		Projectile()
		
		void touchTarget();

		virtual Component::Category getCategory() const;

	private:
		bool hasTouchedTarget_();
	};


} // namespace eg


#endif // EG_COMPONENTTRACKER_HPP
